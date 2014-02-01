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
    double start_time;
    double active_duration;
}channel_slice;

void add_channel(int channel,int num_slices, int startTime,struct channel *cArray,struct channel_slice *sArray)
{
    int n;
    cArray[channel].num_viewers++;  
    if(cArray[channel].assigned == FALSE)
    {
        cArray[channel].assigned = TRUE;
        for(n = 0; n<num_slices; n++)
        {
            if(sArray[n].assigned == FALSE)
            {
                sArray[n].assigned = TRUE;
                sArray[n].channel = channel;
                sArray[n].start_time = startTime;
                cArray[channel].channel_slice = n;
                break;
            }
        }
    }
}
void remove_channel(int channel,int endTime,struct channel *cArray,struct channel_slice *sArray)
{
    int slice_num;
    cArray[channel].num_viewers--;                    
    if(cArray[channel].num_viewers == 0)
    {
        cArray[channel].assigned = FALSE;
        slice_num = cArray[channel].channel_slice;
        sArray[slice_num].assigned = FALSE;
        sArray[slice_num].channel = -1;
        sArray[slice_num].active_duration += endTime-sArray[slice_num].start_time;
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
    for(k = 0; k <num_users; k++)
    {
        time = rand()%100;
        channel = zipfRN(alpha,num_channels);
        userArray[k].user_state = FLIP;
        userArray[k].nextEvent_time = time + exponentialRN(1/FLIP_MEAN);
        add_channel(channel,num_slices,time,channelArray,sliceArray);
        userArray[k].current_channel = channel;
        userArray[k].num_flips = exponentialRN(1/CHANNEL_MEAN);
    }
    
    int num_flips, slice_num;
    for(k = 0; k<simulation_time; k++)
    {
        for(m = 0; m<num_users; m++)
        {
            if(userArray[m].nextEvent_time <= k)
            {
                //User moves to the Flippping state
                if(userArray[m].user_state == VIEW)
                {   
                    //Removing the current channel
                    channel = userArray[m].current_channel;
                    remove_channel(channel,k,channelArray,sliceArray);
                    
                    //The number of channels the user will flip through
                    num_flips = exponentialRN(1/CHANNEL_MEAN);                    
                    userArray[m].num_flips = num_flips;
                    
                    if(userArray[m].num_flips > 0)
                    {
                        //Change to FLIP state
                        userArray[m].user_state = FLIP;
                        
                        //User moves up to the next channel (channel+1)
                        channel = (channel+1)%num_channels;
                        
                        //Viewing Duration (in the Flip State)
                        time = exponentialRN(1/FLIP_MEAN);
                        userArray[m].nextEvent_time = k+time;                    
                        
                        //Adding the next channel                    
                        userArray[m].current_channel = channel;
                        add_channel(channel,num_slices,k,channelArray,sliceArray);                    
                    }
                    else if(userArray[m].num_flips == 0)
                    {    
                        //The new channel user starts viewing
                        channel = zipfRN(alpha,num_channels);
                        time = exponentialRN(1/VIEW_MEAN);                    
                        userArray[m].nextEvent_time = k+time;
                    
                        //Adding the new channel                    
                        userArray[m].current_channel = channel;
                        add_channel(channel,num_slices,k,channelArray,sliceArray);
                    }
                }
                
                //User continues in the Flipping State
                else if(userArray[m].user_state == FLIP && userArray[m].num_flips > 0)
                {                   
                    //Removing the current channel
                    channel = userArray[m].current_channel;
                    remove_channel(channel,k,channelArray,sliceArray);
                    
                    //The number of remaining channels the user will flip through                    
                    userArray[m].num_flips--;
                    
                    if(userArray[m].num_flips > 0)
                    {
                        //User moves up to the next channel (channel+1)
                        channel = (channel+1)%num_channels;
                        
                        //Viewing Duration (in the Flip State)
                        time = exponentialRN(1/FLIP_MEAN);
                        userArray[m].nextEvent_time = k+time;                    
                        
                        //Adding the next channel                    
                        userArray[m].current_channel = channel;
                        add_channel(channel,num_slices,k,channelArray,sliceArray);                    
                    }
                    else if(userArray[m].num_flips == 0)
                    {    
                        //Moves to VIEW State
                        userArray[m].user_state = VIEW;
                    
                        //The new channel user starts viewing
                        channel = zipfRN(alpha,num_channels);
                        time = exponentialRN(1/VIEW_MEAN);                    
                        userArray[m].nextEvent_time = k+time;
                    
                        //Adding the new channel                    
                        userArray[m].current_channel = channel;
                        add_channel(channel,num_slices,k,channelArray,sliceArray);
                    }
                }
                
                //User finishes flipping; moves to the Viewing State
                else if(userArray[m].user_state == FLIP && userArray[m].num_flips == 0)
                {
                    //Moves to VIEW State
                    userArray[m].user_state = VIEW;
                    
                    //Removing the current channel 
                    channel = userArray[m].current_channel;
                    remove_channel(channel,k,channelArray,sliceArray);
                    
                    //The new channel user starts viewing
                    channel = zipfRN(alpha,num_channels);
                    time = exponentialRN(1/VIEW_MEAN);                    
                    userArray[m].nextEvent_time += time;
                    
                    //Adding the new channel                    
                    userArray[m].current_channel = channel;
                    add_channel(channel,num_slices,k,channelArray,sliceArray);
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
            printf("Slice:%d Active Duration:%f Utilization:%f\n",k,sliceArray[k].active_duration,sliceArray[k].active_duration/simulation_time);
            avg_streams += sliceArray[k].active_duration/simulation_time;
        }
    }
    printf("\n\n");
    //avg_streams = avg_streams/active_slices;
    printf("Average Number of Streams: %f\n",avg_streams);
    
    return 0;
}
