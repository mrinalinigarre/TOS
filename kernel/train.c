/*

Name: MRINALINI GARRE

*/


#include <kernel.h>

#define ticks 30
#define time 35

/*String Concatenation*/
void str_concat(char* str1,char* str2){

	int i=k_strlen(str1);
	int j=0;
	while(i<(k_strlen(str2)+k_strlen(str1))){
		str1[i]=str2[j];
		i++;j++;
	}
}

/*Sending message to com process*/
void send_com(int window_id,char* cmd){
	
	char buffer[20]="\0";
	COM_Message msg;
	sleep(3);
	
	str_concat(buffer,cmd);
	str_concat(buffer,"\015");
	msg.output_buffer = buffer;
	msg.input_buffer = NULL;
	msg.len_input_buffer = 0;
	
	//wm_print(window_id,"Command: %s\n",msg);
	send(com_port,&msg);
}

/*Function to change the switches*/
void set_switch(int window_id,char* number,char* color){

	char buffer[20]="\0";
	char cmd[20]="\0";
	str_concat(cmd,"M");
	str_concat(cmd,number);	
	str_concat(cmd,color);
	
	COM_Message msg;
	sleep(3);
	
	str_concat(buffer,cmd);
	str_concat(buffer,"\015");
	msg.output_buffer = buffer;
	msg.input_buffer = NULL;
	msg.len_input_buffer = 0;
	
	wm_print(window_id,"Changed Switch: %s to %s\n",number,color);
	send(com_port,&msg);
	

}

/*Intializing the intial switches*/
void init_switches(int window_id){

	wm_print(window_id,"Intialized Switches\n");
	set_switch(window_id,"8","G");
	set_switch(window_id,"4","G");
	set_switch(window_id,"5","G");
	set_switch(window_id,"9","R");
	set_switch(window_id,"1","G");

}

/*Checking if train/wagon is on the track*/
int check_track(int window_id,char* cmd){

	send_com(window_id,"R");
	char buffer[3];
	COM_Message msg;
	char command[20]="\0";
	sleep(3);

	str_concat(command,cmd);
	str_concat(command,"\015");
	msg.output_buffer = command;
	msg.input_buffer = buffer;
	msg.len_input_buffer = 3;
	for(int i=0;i<10;i++){
	}

	send(com_port,&msg);
	if(buffer[1]=='1'){
		wm_print(window_id,"Probe track: %s\n",cmd);
		return 1;
	}else{
		return 0;
	}	

}

/*Function for checking the configuration*/
int check_config(int window_id){
	
	if(check_track(window_id,"C8")==1 && check_track(window_id,"C11")==1){
		return 1;	
	}
	else if(check_track(window_id,"C12")==1 && check_track(window_id,"C2")==1){
		return 2;	
	}
	else if(check_track(window_id,"C2")==1 && check_track(window_id,"C11")==1){
		return 3;	
	}
	else if(check_track(window_id,"C5")==1 && check_track(window_id,"C12")==1){
		return 4;	
	}
	else
		return 0;	

}

/*Checking the status of the track if it has train/zamboni on it*/
int check_track_status(int window_id,char* cmd,int seconds){

	int i,a=0;
	for(i=0;i<seconds;i++){
		a=check_track(window_id,cmd);
		if(a==1){	
			
			break;
		}
	}
	if(a==1){
		return 1;
	}else
		return 0;
}

/*Stopping Train*/
void stop_train(int window_id){
	
	wm_print(window_id,"Stopped the train\n");
	send_com(window_id,"L20S3");
	send_com(window_id,"L20S0");
}

/*Changing the direction of train*/
void change_direction(int window_id){

	stop_train(window_id);
	send_com(window_id,"L20D");
	wm_print(window_id,"Changed the direction of train\n");
}

/*Configuration1 with or without zamboni*/
void configuration1(int window_id,int zamboni){

	wm_print(window_id,"Inside Configuration1\n");
	set_switch(window_id,"5","R");
	set_switch(window_id,"6","G");
	set_switch(window_id,"7","R");
	if(zamboni==1 && check_track_status(window_id,"C6",time)){
		send_com(window_id,"L20S5");	
	}
	else if(zamboni==0){
		send_com(window_id,"L20S5");
	}
	if(check_track_status(window_id,"C7",time)){
		change_direction(window_id);
	}
	send_com(window_id,"L20S5");
	if(check_track_status(window_id,"C12",time)){
		
		change_direction(window_id);
	}
	if(zamboni==1 && check_track_status(window_id,"C10",time)){
		send_com(window_id,"L20S5");
	}else if(zamboni == 0){
		send_com(window_id,"L20S5");
	}
	if(check_track_status(window_id,"C13",time)){
		//sleep(30);
		change_direction(window_id);
	}
	if(zamboni==1 && check_track_status(window_id,"C6",time)){
		send_com(window_id,"L20S5");
		set_switch(window_id,"6","R");
	}else if(zamboni==0){
		send_com(window_id,"L20S5");
		set_switch(window_id,"6","R");
	}
	if(check_track_status(window_id,"C7",time)){
		change_direction(window_id);
	}
	set_switch(window_id,"8","R");
	set_switch(window_id,"2","R");
	set_switch(window_id,"1","R");
	send_com(window_id,"L20S5");
	if(check_track_status(window_id,"C8",time)){
		stop_train(window_id);
	}
	wm_print(window_id,"Configuration1: Successfully completed the task \n");
	become_zombie();

}

/*Configuration2 with or without zamboni*/
void configuration2(int window_id,int zamboni){

	wm_print(window_id,"Inside Configuration2\n");

	  set_switch(window_id,"2","G");
	  send_com(window_id,"L20S5");
	  //while running might have to change ticks accorcingly
  	  sleep(3*ticks);
	  send_com(window_id,"L20S4");
	  if(check_track_status(window_id,"C14",time)){
		change_direction(window_id);
	  }
	  set_switch(window_id,"1","R");
	  sleep(ticks);
	  send_com(window_id,"L20S5");
	  if(check_track_status(window_id,"C1",time)){
		set_switch(window_id,"1","G");
	 	stop_train(window_id);
	  }
	  if(zamboni == 1 && check_track_status(window_id,"C6",time)){
	        send_com(window_id,"L20S5");
	  }else if(zamboni == 0){
		send_com(window_id,"L20S5");
	  }
	  if(zamboni==1 && check_track_status(window_id,"C10",time)){
		set_switch(window_id,"5","R");
		set_switch(window_id,"6","G");
	  stop_train(window_id);
	  sleep(ticks);
	  send_com(window_id,"L20S4");
	  
	  }else if(zamboni==0){
		set_switch(window_id,"5","R");
		set_switch(window_id,"6","G");		
	  }	  
	  if(check_track_status(window_id,"C12",time)){
		set_switch(window_id,"5","G");
		stop_train(window_id);
	  }
	  wm_print(window_id,"Configure2: Wagon is in safe zone ");
	become_zombie();
}

/*Configuration3 with or without zamboni*/
void configuration3(int window_id,int zamboni){

	wm_print(window_id,"Inside Configuration3\n");
	
	  set_switch(window_id,"1","R");
	  set_switch(window_id,"2","R");
	  set_switch(window_id,"7","R");
	  send_com(window_id,"L20D");
	  send_com(window_id,"L20S5");
	  //check for number of ticks might have to change here
	  if(check_track_status(window_id,"C1",time)){
		sleep(3*ticks);
		send_com(window_id,"L20S4");
	  }
	  if(check_track_status(window_id,"C14",time)){
		change_direction(window_id);
	  }
	  send_com(window_id,"L20S5");
	  if(check_track_status(window_id,"C12",time)){
	  	set_switch(window_id,"1","G");
		stop_train(window_id);
	  }
	  if(zamboni==1 && check_track_status(window_id,"C13",time)){
		send_com(window_id,"L20S5");
	  }else if(zamboni==0){
		send_com(window_id,"L20S5");
		set_switch(window_id,"1","R");
		set_switch(window_id,"2","G");
	  }
	  if(zamboni==1 && check_track_status(window_id,"C3",time)){
		set_switch(window_id,"1","R");
		set_switch(window_id,"2","G");
	  }
	  if(check_track_status(window_id,"C2",time)){
		set_switch(window_id,"1","G");
		stop_train(window_id);
	  }
	  wm_print(window_id,"Configuration3: Yayyyy escaped from Zamboni :)\n"); 
	become_zombie();
}

/*Configuration4 with or without zamboni*/
void configuration4(int window_id,int zamboni){

	wm_print(window_id,"Inside Configuration4\n");
	set_switch(window_id,"4","R");
	set_switch(window_id,"3","G");
	send_com(window_id,"L20S5");
	if(check_track_status(window_id,"C6",time)){
		change_direction(window_id);
	}
	send_com(window_id,"L20S5");
	if(check_track_status(window_id,"C2",time)){
		set_switch(window_id,"4","G");
		set_switch(window_id,"1","R");
		set_switch(window_id,"2","R");
		set_switch(window_id,"7","R");
		stop_train(window_id);
	}
	if(zamboni == 1 && check_track_status(window_id,"C13",time)){
		send_com(window_id,"L20S5");
	}else if(zamboni == 0){
		send_com(window_id,"L20S5");
	}
 	if(check_track_status(window_id,"C1",time)){
		sleep(3*ticks);
		send_com(window_id,"L20S4");
	}
	if(check_track_status(window_id,"C14",time)){
		change_direction(window_id);
	}
	send_com(window_id,"L20S5");
	if(check_track_status(window_id,"C12",time)){
		sleep(ticks);
		send_com(window_id,"L20S4");
	}
	if(zamboni==1 && check_track_status(window_id,"C11",time)){
		stop_train(window_id);
	}
	if(zamboni==1 && check_track_status(window_id,"C10",time)){
		send_com(window_id,"L20S5");
	}
	if(check_track_status(window_id,"C13",time)){
		change_direction(window_id);
	}
	send_com(window_id,"L20S5");
	if(zamboni==1 && check_track_status(window_id,"C4",time)){
		set_switch(window_id,"4","R");
		set_switch(window_id,"3","R");
	}else if(zamboni==0){
		set_switch(window_id,"4","R");
		set_switch(window_id,"3","R");
	}
	//see and change what if train is running too slow
	sleep(ticks);
	send_com(window_id,"L20S4");
	if(check_track_status(window_id,"C5",time)){
		stop_train(window_id);
		set_switch(window_id,"4","G");
	}
	wm_print(window_id,"Configuration4: Hurrayyy Won the Game!!\n");
	become_zombie();
}

/*Main Train Process*/
void train_process(PROCESS self, PARAM param)
{
	int config,zamboni;
	int window_id = wm_create(1,1,40,15);

	init_switches(window_id);
	config = check_config(window_id);
	wm_print(window_id,"-------------------\n");
	wm_print(window_id,"Configuration : %d\n",config);
	wm_print(window_id,"-------------------\n");
	zamboni = check_track_status(window_id,"C4",35);
	wm_print(window_id,"Zamboni value: %d\n",zamboni);

	switch(config){
		
		case 1: configuration1(window_id,zamboni);
			break;
		case 2: configuration2(window_id,zamboni);
			break;
		case 3: configuration3(window_id,zamboni);
			break;
		case 4: configuration4(window_id,zamboni);
			break;
		case 0: wm_print(window_id,"Invalid Configuration\n");
			break;
	}
	while(1){
	   resign();	
	}

}


void init_train()
{

	create_process(train_process,5,0,"Train");
	resign();
}
