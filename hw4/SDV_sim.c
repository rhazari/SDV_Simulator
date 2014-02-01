#include<stdio.h>
#include <stdlib.h>
#include <variates.h>
#include <stdbool.h>
#include <time.h>

typedef enum {VIEW, FLIP} state;

typedef struct user{
    state user_state;
    int num_flips;
    int current_channel;
    int nextEvent_time;
    int fail_request;
    int total_request;
}user;

typedef struct channel{
    bool assigned;
    int channel_slice;
    int num_viewers;
}channel;

typedef struct channel_slice{
    bool assigned;
    int channel;
    int start_time;
    int active_duration;
}channel_slice;

int peak_slice_count = 0;
int slice_count = 0;
int viewers_count = 0;

int add_channel(int channel,int num_slices, int startTime,struct channel *cArray,struct channel_slice *sArray, FILE *fp)
{
    int n, success = 0;    
    if(cArray[channel].assigned == FALSE)
    {
        for(n = 0; n<num_slices; n++)
        {
            if(sArray[n].assigned == FALSE)
            {
                slice_count++;
                fprintf(fp,"%f  %d  %d\n",(double)(startTime)/10, slice_count, viewers_count);
                if(n>peak_slice_count)
                    peak_slice_count = n;
                cArray[channel].assigned = TRUE;
                sArray[n].assigned = TRUE;
                sArray[n].channel = channel;
                sArray[n].start_time = startTime;
                cArray[channel].channel_slice = n;
                cArray[channel].num_viewers++;
                success = 1;
                break;
            }
        }
    }
    else if(cArray[channel].assigned == TRUE)
    {
        cArray[channel].num_viewers++;
        success = 1;
    }
    return success;
}
void remove_channel(int channel,int endTime,struct channel *cArray,struct channel_slice *sArray, FILE* fp)
{
    int slice_num;
    cArray[channel].num_viewers--;                    
    if(cArray[channel].num_viewers == 0)
    {
        cArray[channel].assigned = FALSE;
        slice_num = cArray[channel].channel_slice;
        slice_count--;
        if(slice_num > 0)
        { 
            fprintf(fp,"%f  %d  %d\n",(double)(endTime)/10, slice_count, viewers_count);
            sArray[slice_num].assigned = FALSE;
            sArray[slice_num].channel = -1;
            sArray[slice_num].active_duration += endTime-sArray[slice_num].start_time;
        }
    }
}

int main(int argc, char* argv[])
{
    if(argc != 6)
    {
        printf("Enter the required Parameters.\n");
        printf("<Npmr> <Nc> <Chanel Slices> <simulation duration> <zipf parameter>\n");
        return 2;
    }
    
    FILE *fp = fopen("file.txt","w");
    if(fp == NULL)
    {
        printf("Error in opening file. Exiting...\n");
        exit(1);
    }
    
    //Time duration in Decisecond
    double VIEW_MEAN = 7200;
    double FLIP_MEAN = 100;
    double CHANNEL_MEAN = 6;
    
    srand(time(NULL));
    
    //Number of peak multicast request
    int num_users = atoi(argv[1]);
    //Number of channels           
    int num_channels = atoi(argv[2]);
    //Number of channels handled by SDV
    int num_slices = atoi(argv[3]);
    //The simulation runtime
    int simulation_time = atoi(argv[4]);
    //Simulation time in decisecond
    simulation_time = simulation_time*10;
    //The zipf shape parameter
    float alpha = atof(argv[5]);
    
    user *userArray = (user *)malloc(num_users * sizeof(user));
    channel *channelArray = (channel *)malloc(num_channels * sizeof(channel));
    channel_slice *sliceArray = (channel_slice *)malloc(num_slices * sizeof(channel_slice));
    
    int k,m,n;
    int num_slice = 0;
    //Initializing the Channel struct
    for(k = 0; k<num_channels; k++)
    {
        channelArray[k].assigned = FALSE;
        channelArray[k].channel_slice = -1;
        channelArray[k].num_viewers = 0;
    }
    //Initializing the Channel Slice struct
    for(k = 0; k<num_slices; k++)
    {
        sliceArray[k].assigned = FALSE;
        sliceArray[k].channel = -1;
        sliceArray[k].active_duration = 0;
        sliceArray[k].start_time = 0;
    }
    
    int channel, time;
    int num_flips;
    for(k = 0; k <num_users; k++)
    {
        time = rand()%100;
        channel = zipfRN(alpha,num_channels);
        userArray[k].total_request = 1;
        userArray[k].fail_request = 0;
        userArray[k].user_state = FLIP;
        while(!add_channel(channel,num_slices,time,channelArray,sliceArray,fp))
        {
            channel = zipfRN(alpha,num_channels);
            userArray[k].fail_request++;
            userArray[k].total_request++;            
        }
        userArray[k].current_channel = channel;
        userArray[k].nextEvent_time = time + exponentialRN(1/FLIP_MEAN);
        num_flips = exponentialRN(1/CHANNEL_MEAN);
        while(num_flips == 0)
        {
            num_flips = exponentialRN(1/CHANNEL_MEAN);
        }
        userArray[k].num_flips = num_flips;
    }
    
    int flag;
    for(k = 0; k<simulation_time; k++)
    {
        for(m = 0; m<num_users; m++)
        {
            if(userArray[m].nextEvent_time <= k)
            {
                //User is in the VIEW state
                if(userArray[m].user_state == VIEW)
                {   
                    //Removing the current channel
                    channel = userArray[m].current_channel;
                    if(channel != -1)
                    {
                        remove_channel(channel,k,channelArray,sliceArray,fp);
                    }
                    //The number of channels the user will flip through
                    num_flips = exponentialRN(1/CHANNEL_MEAN);
                    while(num_flips == 0)
                    {
                        num_flips = exponentialRN(1/CHANNEL_MEAN);
                    }                  
                    userArray[m].num_flips = num_flips;
                    
                    //Change to FLIP state
                    userArray[m].user_state = FLIP;
                    viewers_count--;
                    //User moves up to the next channel
                    channel = rand()%num_channels;
                        
                    userArray[m].total_request++;
                    //Adding a new channel
                    flag = add_channel(channel,num_slices,k,channelArray,sliceArray,fp);
                    if(flag)
                    {
                        userArray[m].nextEvent_time += exponentialRN(1/FLIP_MEAN);
                        userArray[m].current_channel = channel;            
                    }
                    else if(!flag)
                    {
                        userArray[m].current_channel = -1; 
                        userArray[m].fail_request++; 
                    }                                                                           
                }
                
                //User is in the FLIP state
                else if(userArray[m].user_state == FLIP)
                {                   
                    //Removing the current channel
                    channel = userArray[m].current_channel;
                    if(channel != -1)
                    {
                        remove_channel(channel,k,channelArray,sliceArray,fp);
                        //The number of remaining channels the user will flip through                    
                        userArray[m].num_flips--;
                    }
                                       
                    if(userArray[m].num_flips > 0)
                    {
                        //User moves up to the next channel (channel+1)
                        channel = rand()%num_channels;
                        
                        userArray[m].total_request++;
                        //Adding a new channel
                        flag = add_channel(channel,num_slices,k,channelArray,sliceArray,fp); 
                        if(flag)
                        {
                            //Viewing Duration (in the Flip State)
                            userArray[m].nextEvent_time += exponentialRN(1/FLIP_MEAN);                                            
                            //Adding the next channel                    
                            userArray[m].current_channel = channel;            
                        }
                        else if (!flag)
                        {
                            userArray[m].current_channel = -1; 
                            userArray[m].fail_request++;
                        }               
                    }
                    else if(userArray[m].num_flips <= 0)
                    {    
                        //Moves to VIEW State
                        userArray[m].user_state = VIEW;
                        viewers_count++;
                        
                        //The new channel user starts viewing
                        channel = zipfRN(alpha,num_channels);
                        userArray[m].total_request++;
                        //Adding a new channel
                        flag = add_channel(channel,num_slices,k,channelArray,sliceArray,fp); 
                        if(flag)
                        {
                            userArray[m].nextEvent_time += exponentialRN(1/VIEW_MEAN);                   
                            userArray[m].current_channel = channel;          
                        }
                        else if (!flag)
                        {
                            userArray[m].current_channel = -1; 
                            userArray[m].fail_request++;
                        }                    
                    }
                }   
            }
        }
    }
    
    int active_slices = 0;
    double avg_streams = 0;
    for(k = 0; k<num_slices; k++)
    {
        if(sliceArray[k].assigned == TRUE && sliceArray[k].active_duration == 0)
        {
            sliceArray[k].active_duration = simulation_time - sliceArray[k].start_time;    
        }
        if(sliceArray[k].active_duration > 0)
        {
            active_slices++;
            //printf("Slice:%d Active Duration:%d Utilization:%3.2f\n",k,sliceArray[k].active_duration,(float)(sliceArray[k].active_duration)/simulation_time);
            avg_streams += (float)(sliceArray[k].active_duration)/simulation_time;
        }
    }
    printf("\n***** SDV Simulation *****\n");    
    unsigned int total_request = 0;
    unsigned int fail_request = 0;
    for(k = 0; k< num_users; k++)
    {
        total_request += userArray[k].total_request;
        fail_request += userArray[k].fail_request;
    }
    
    printf("Fail request: %u\n",fail_request);
    printf("Total request: %u\n",total_request);    
    printf("Average Number of Streams: %f\n",avg_streams);
    printf("Peak Number of Slices: %d\n",peak_slice_count);
    printf("Average Blocking Probability: %3.4f\n",(float)(fail_request)/total_request);
    fclose(fp);
    
    return 0;
}
