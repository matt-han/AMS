#include <stdio.h>
#include <math.h>
#include <wait.h>
#include <errno.h>
#include <libplayerc/playerc.h>
#include <cxcore.h>
#include <cv.h>
#include <highgui.h>

#include "include/pathfinder_t.h"
#include <stdio.h>
#include <stdlib.h>

#define DRAW "./draw"
#define center_Y 240
#define center_X 320
#define pix_meter_x 80
#define pix_meter_y 60
#define white 255
#define img_width 800
#define img_height 600	
#define depth 8
#define channels 1	
#define grad_45 45*M_PI/180			//45° in Rad
#define rotDiff 0.01						//Differenzwinkel
#define entWinkel 45						//Entscheidungswinkel für Nachbarschaften
#define leftRightWinkel 180/M_PI		//Entscheidungswinkel ob Links oder Rechtsdrehung
#define posNegWinkel 360				//wird benötigt, um negative Winkel zu positivieren
#define winkelGeschw 30.0
#define translGeschw 0.1

map_t map;
short *cells;
int x_limit, y_limit;
int st_x, st_y, tar_x, tar_y, st, tar;

int main(int argc, const char **argv)
{
	int i,j,left,right,row,column;
	int angle,mapRow,mapColumn;
	int test=0;
	
	FILE *fp;
	char mapArray[img_height][img_width];
	
	playerc_client_t *client;
	playerc_position2d_t *position2d;
	playerc_laser_t *laser;
	
	int past=0;
	float x,y;
	
	IplImage *orig_image=NULL;
	IplImage *dst2=NULL;
   
   // Zeiger auf Bilddaten
   unsigned char *orig_image_buffer=NULL;
   unsigned char *dst_buffer2=NULL;

   // Fenstertitel (OpenCV HighGUI)
   const char *orig_image_windowname="Originalbild";
       
   // -----------------------------------------------------------------

   // Erstelle Ziel-Bild mit gleicher Groesse und Tiefe
   orig_image=cvCreateImage(cvSize(img_width,img_height),depth,channels);
   dst2=cvCreateImage(cvSize(img_width,img_height),depth,channels);                 

   // Erstellen der Fenster (mit manueller Groessenanpassung)
   //cvNamedWindow(orig_image_windowname,0);  
     
   orig_image_buffer=(unsigned char*)orig_image->imageData;
   dst_buffer2=(unsigned char*)dst2->imageData;
   
   memset(orig_image_buffer,sizeof(unsigned char*),0);
   memset(dst_buffer2,sizeof(unsigned char*),0);
   
	// Create a client and connect it to the server.
	client = playerc_client_create(NULL, "localhost", 6665);
	if (0 != playerc_client_connect(client))
		return -1;

	// Create and subscribe to a position2d device.
	position2d = playerc_position2d_create(client, 0);
	if (playerc_position2d_subscribe(position2d, PLAYER_OPEN_MODE))
		return -1;

	laser = playerc_laser_create(client, 0);
	if (playerc_laser_subscribe(laser, PLAYER_OPEN_MODE) != 0) { 
		printf("laser error!\n");
		exit(1);
	}	
		
	int searchX=1;
	int next=0;
	int oneTime=1;
	int rowStart, colStart;
	
	while(1)
	{
		
		// Wait for new data from server
		playerc_client_read(client);		

		//get robot start-position
		int xRobStart=0;
		int yRobStart=0;
					
		
		double robWinkel=0;
		if(position2d->pa >= 0)
		{
			robWinkel = position2d->pa*leftRightWinkel;
		}else
		{
			robWinkel = posNegWinkel+(position2d->pa*leftRightWinkel);
		}
		switch(next)
		{
		case 1:	// 135°
			if(position2d->pa <3*grad_45-rotDiff || position2d->pa >3*grad_45+rotDiff)
			{
				if(3*entWinkel<=robWinkel && robWinkel<7*entWinkel)
				{
					playerc_position2d_set_cmd_vel(position2d, 0, 0, DTOR(-winkelGeschw), 1);
				}else
				{
					playerc_position2d_set_cmd_vel(position2d, 0, 0, DTOR(winkelGeschw), 1);
				}
			}else
			{
				playerc_position2d_set_cmd_vel(position2d, translGeschw, translGeschw, 0.0, 1);
				searchX=1;
			}
			break;
			
		case 2:	// 90°
			if(position2d->pa <2*grad_45-rotDiff || position2d->pa >2*grad_45+rotDiff)
			{
				if(2*entWinkel<=robWinkel && robWinkel<6*entWinkel)
				{
					playerc_position2d_set_cmd_vel(position2d, 0, 0, DTOR(-winkelGeschw), 1);
				}else
				{
					playerc_position2d_set_cmd_vel(position2d, 0, 0, DTOR(winkelGeschw), 1);
				}
				
			}else
			{
				playerc_position2d_set_cmd_vel(position2d, translGeschw, translGeschw, 0.0, 1);
				searchX=1;
			}
			break;

		case 3:	// 45°		
			if(position2d->pa <1*grad_45-rotDiff || position2d->pa >1*grad_45+rotDiff)
			{
				if(1*entWinkel<=robWinkel && robWinkel<5*entWinkel)
				{
					playerc_position2d_set_cmd_vel(position2d, 0, 0, DTOR(-winkelGeschw), 1);
				}else
				{
					playerc_position2d_set_cmd_vel(position2d, 0, 0, DTOR(winkelGeschw), 1);
				}
		
			}else
			{
				playerc_position2d_set_cmd_vel(position2d, translGeschw, translGeschw, 0.0, 1);
				searchX=1;
			}
			break;		
			
		case 4:	// 180°
			if(position2d->pa <4*grad_45-rotDiff && position2d->pa >-4*grad_45+rotDiff)
			{
				if(4*entWinkel<=robWinkel && robWinkel<8*entWinkel)
				{
					playerc_position2d_set_cmd_vel(position2d, 0, 0, DTOR(-winkelGeschw), 1);
				}else
				{
					playerc_position2d_set_cmd_vel(position2d, 0, 0, DTOR(winkelGeschw), 1);
				}
			}else
			{
				playerc_position2d_set_cmd_vel(position2d, translGeschw, translGeschw, 0.0, 1);
				searchX=1;
			}
			break;		
			
		case 6:	// 0°
			if(position2d->pa <0*grad_45-rotDiff || position2d->pa >0*grad_45+rotDiff)
			{
				if(0*entWinkel<=robWinkel && robWinkel<4*entWinkel)
				{
					playerc_position2d_set_cmd_vel(position2d, 0, 0, DTOR(-winkelGeschw), 1);
				}else
				{
					playerc_position2d_set_cmd_vel(position2d, 0, 0, DTOR(winkelGeschw), 1);
				}
				
			}else
			{
				playerc_position2d_set_cmd_vel(position2d, translGeschw, translGeschw, 0.0, 1);
				searchX=1;
			}
			break;

		case 7:	// 225°
			if(position2d->pa <-3*grad_45-rotDiff || position2d->pa >-3*grad_45+rotDiff)
			{
				if(5*entWinkel<=robWinkel && robWinkel<1*entWinkel)
				{
					playerc_position2d_set_cmd_vel(position2d, 0, 0, DTOR(-winkelGeschw), 1);
				}else
				{
					playerc_position2d_set_cmd_vel(position2d, 0, 0, DTOR(winkelGeschw), 1);
				}
			}else
			{
				playerc_position2d_set_cmd_vel(position2d, translGeschw, translGeschw, 0.0, 1);
				searchX=1;
			}
			break;
		case 8:	// 270°
			if(position2d->pa <-2*grad_45-rotDiff || position2d->pa >-2*grad_45+rotDiff)
			{
				if(2*entWinkel>robWinkel || robWinkel>=6*entWinkel)
				{
					playerc_position2d_set_cmd_vel(position2d, 0, 0, DTOR(-winkelGeschw), 1);
				}else
				{
					playerc_position2d_set_cmd_vel(position2d, 0, 0, DTOR(winkelGeschw), 1);
				}
			}else
			{
				playerc_position2d_set_cmd_vel(position2d, translGeschw, translGeschw, 0.0, 1);
				searchX=1;
			}
			break;
		case 9:	// 315°
			if(position2d->pa <-1*grad_45-rotDiff || position2d->pa >-1*grad_45+rotDiff)
			{
				if(7*entWinkel<=robWinkel || robWinkel<3*entWinkel)
				{
					playerc_position2d_set_cmd_vel(position2d, 0, 0, DTOR(-winkelGeschw), 1);
				}else
				{
					playerc_position2d_set_cmd_vel(position2d, 0, 0, DTOR(winkelGeschw), 1);
				}
			}else
			{
				playerc_position2d_set_cmd_vel(position2d, translGeschw, translGeschw, 0.0, 1);
				searchX=1;
			}
			break;
		default: playerc_position2d_set_cmd_vel(position2d, 0.0, 0.0, 0.0, 1);
		}
		
		//cvResizeWindow(orig_image_windowname,   orig_image->width,
   	                                      //  orig_image->height);                                          
      //cvShowImage(orig_image_windowname,dst2);
            
      cvWaitKey(3);   		
	}

	// Shutdown
	playerc_position2d_unsubscribe(position2d);
	playerc_position2d_destroy(position2d);

	playerc_laser_unsubscribe(laser);
	playerc_laser_destroy(laser);

	playerc_client_disconnect(client);
	playerc_client_destroy(client);

	return 0;
}
