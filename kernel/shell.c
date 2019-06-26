/*
 Name: Mrinalini Garre
*/

#include <kernel.h>


#define Shell_window_width 60
#define Shell_window_height 15
# define max_size 50
#define M 20
#define N 50

char userinput[max_size]="";
int k;
char buffer[10]="";
char input[15];



/*ps function*/
void ps(int window_id){

	int i;
    	PCB* p = pcb;
	wm_print(window_id,"Current TOS Processes\n");
	wm_print(window_id,"------------------------------------------------\n");
	for (i = 0; i < MAX_PROCS; i++, p++) {
		if (!p->used){
	    		continue;
		}
	  wm_print(window_id, " %s\n", p->name);
	}
	
}

/*ps execution*/
void ps_exec(int window_id, PCB* p){
	static const char *state[] = 
	{ "READY          ",
	  "SEND_BLOCKED   ",
	  "REPLY_BLOCKED  ",
	  "RECEIVE_BLOCKED",
	  "MESSAGE_BLOCKED",
	  "INTR_BLOCKED   "
	};
        if (!p->used) {
		wm_print(window_id, "PCB slot unused!\n");
		return;
    	}
    	wm_print(window_id, state[p->state]);
    	if (p == active_proc)
		wm_print(window_id, " *      ");
    	else
		wm_print(window_id, "        ");
    	wm_print(window_id, "  %2d", p->priority);
    	wm_print(window_id, " %s\n", p->name);

}


/*TOS process state*/
void ps_details(int window_id){
	int i;
    	PCB* p = pcb;
	wm_print(window_id,"State           Active Prio Name\n");
	wm_print(window_id,"------------------------------------------------\n");
	
	for (i = 0; i < MAX_PROCS; i++, p++) {
		if (!p->used)
	    		continue;
		ps_exec(window_id, p);
	}
}

/*clear window function*/
void clear_screen(int window_id){
	wm_clear(window_id);
}


/* new shell function*/
void create_new_shell(){
	start_shell();
}

/*help function*/
void help(int window_id){
	wm_print(window_id,"-------------------------------------------------\n");
	wm_print(window_id, "TOS shell commands: \n");
	wm_print(window_id,"-------------------------------------------------\n");
	wm_print(window_id, "1.about         Prints a message\n");
	wm_print(window_id, "2.clear         Clears the shell\n");
	wm_print(window_id, "3.echo          Print string to the shell\n");
	wm_print(window_id, "4.help          Print the list of commands\n");
	wm_print(window_id, "5.pong          Opens the pong game\n");
	wm_print(window_id, "6.ps            Prints the names of all processes \n");
	wm_print(window_id, "7.ps -d         Prints the details of all processes\n");
	wm_print(window_id, "8.shell         Opens another shell\n");
	wm_print(window_id, "9.history       Prints history of commands\n");
	wm_print(window_id, "10.!<number>    Runs the command from history\n");
	wm_print(window_id,"-------------------------------------------------\n");
}


/* Pong game function */
void launch_pong(int window_id){
	start_pong();
}
/*About function*/
void about(int window_id){
 	wm_print(window_id,"Hi this is Mrinalini!\n");
}

/*String Compare function*/
BOOL str_compare(char* first_string, char* second_string){
	int i = 0;
	while(first_string[i] == second_string[i] && first_string[i] != '\0'){
		i++;
	}
	if(first_string[i] != second_string[i]){
		return FALSE;
	}
	else{
		return TRUE;
	}
}
/*echo function*/
void echo(int window_id,char* command){
	
	char* temp = command;
	wm_print(window_id,"%s\n",temp);
}

/*printing history function*/
void show_history(int window_id, char history[M][N],int k){
	
	int l=0;
	while(l<k){
		wm_print(window_id, "\n%d  ",l);
		wm_print(window_id,"%s\n",history[l]);
		l++;
	}
}

/*!<number> function*/
void command_repeat(int window_id, char history[M][N],int k,char* command){
	
	int m = *(command) - '0';	
	if(m==k-1)
	{
		wm_print(window_id,"Out of History Bound\n");
	}
	else{
		parse_semicolon(window_id,history[m],history,k);
			
	}
}


/*trimming white spaces*/
char* trim_white_space(int window_id, char raw_input[]){
	
	//wm_print(window_id,"Raw_input: %s\n",raw_input);
	char *front_trimmed_str = (char *) malloc(max_size);
	char *trimmed_str = (char *) malloc(max_size);
	int front_index=0;
	int j;
	while(raw_input[front_index]==' '){
		front_index++;	
	}
	if(front_index != 0){
		j = 0;
		while(front_index < k_strlen(raw_input)){
			front_trimmed_str[j] = raw_input[front_index];
			j++;
			front_index++;
		}	
	}else{
		j=0;
		while(j<k_strlen(raw_input)){
			front_trimmed_str[j] = raw_input[j];
			j++;
		}
	}
	//wm_print(window_id,"Front :%s\n",front_trimmed_str);
	//trimming rear white spaces
	int rear_index = k_strlen(front_trimmed_str)-1;
	while(front_trimmed_str[rear_index]==' '){
		rear_index--;
	}
	j = 0;
	while(j <= rear_index){
		trimmed_str[j] = front_trimmed_str[j];
		j++;
	}
	return trimmed_str;
}


/*Function to execute each command of TOS*/
void get_command(int window_id, char* temp,char history[M][N],int index){

	int i;
	for(i=0;i<5;i++){
		buffer[i]=*(temp+i);	
	}
	if(k_strlen(temp)>max_size){
	    	wm_print(window_id, "Buffer Overflow\n");
	}
	else if(str_compare(temp,"clear")){
		//wm_print(window_id,"checking clear screen \n");
		clear_screen(window_id);
	}
	else if (str_compare(temp,"about")){
		//wm_print(window_id,"checking about \n");
		about(window_id);
	}
	else if(str_compare(temp,"pong")){
		//wm_print(window_id,"checking pong game \n");
		launch_pong(window_id);
	}
	else if(str_compare(temp,"shell")){
		//wm_print(window_id,"checking new shell \n");
		create_new_shell();	
	}
	else if(str_compare(temp, "ps")){
		ps(window_id);
	}
	else if(str_compare(temp,"ps -d")){
		ps_details(window_id);	
	}
	else if(str_compare(temp, "help")){
		help(window_id);
	}
	else if(str_compare(buffer,"echo ")){
		echo(window_id,temp+5);
	}
	else if(str_compare(temp,"history")){
		show_history(window_id,history,index);	
	}
	else if(*temp=='!'){
		command_repeat(window_id,history,index,temp+1);
	}else
		wm_print(window_id, "Please give valid command\n");
}



/*Function for parsing input command with semicolon*/
void parse_semicolon(int window_id,char userinput[],char history[M][N],int k){
		
		int userinput_index = 0;
		int sc_index = 0;
		char single_command[max_size];
		while(userinput_index < k_strlen(userinput)){
			if(userinput[userinput_index] == ';' ){
				// execute the single command
		 		get_command(window_id,single_command,history,k);
				single_command[sc_index] = '\0';
				sc_index = 0;
			}
			else{
				//store to a new variable
				single_command[sc_index] = userinput[userinput_index];
				sc_index++;
			}
			userinput_index++;
		}
		if(userinput[k_strlen(userinput)-1]!=';'){
			single_command[sc_index]='\0';
		}
		get_command(window_id,single_command,history,k);
		
}


/*Intial shell process*/
void shell_process(PROCESS process, PARAM param){
	
	char key;	
	// for creating window
	int window_id = wm_create(1,1,Shell_window_width, Shell_window_height);

	wm_print(window_id, "-----TOS SHELL by Minni-----\n");
	int index;
	char log[M][N];						//2D array for storing History of commands
	while(1){
		int i=0,cmdlen=0;
		char userinput[max_size]="";
		wm_print(window_id,">>");
		do{
		      key = keyb_get_keystroke(window_id, TRUE);	
		      switch(key){

			   case '\0':
				continue;
			   case '\b':
				if(i==0)
  				  continue;
				i--;
				wm_print(window_id,"%c",key);
				break;
			   case 13:
				wm_print(window_id, "%c", '\n');
				break;
			   default: 
				wm_print(window_id, "%c", key);
				userinput[i] = key;
				i++;
				break;
		       }
			
		}while(key != '\n' && key != '\r');
		char *trimmed_input = trim_white_space(window_id,userinput);
		//wm_print(window_id,"Trimmed_input: %s\n",trimmed_input);

		//Storing History input - log	
		while(cmdlen<k_strlen(trimmed_input)){
			log[index][cmdlen] = trimmed_input[cmdlen];
			cmdlen++;
		}
		index++;
		parse_semicolon(window_id,trimmed_input,log,index);
        }
}

void start_shell(){

	create_process(shell_process,5,0,"Shell");
	resign();
	
}
