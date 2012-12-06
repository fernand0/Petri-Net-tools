/* 
This  program generates PNML files representing Petri Net models as described for the experiments in 
Tricas, F.; Ezpeleta, J.; Computing minimal siphons in Petri net models of resource allocation systems: a parallel solution. Systems, Man and Cybernetics, Part A, IEEE Transactions on Volume 36, Issue 3, May 2006 Page(s):532 - 539
in order to generate examples for the Model Checking Contest 2012 @ Petri Nets.
*/

#include<stdio.h>
#include<string.h>
#include <stdlib.h>

#define element_offset 70                   /*contiguous place-transition separation*/
#define process_index_offset 10000          /*first process-index offset*/
int arc_id=0;                               /*to avoid collisions, arc indexes follow their own path*/
                                        /*place and transition indexes should avoid low values*/


void preface(FILE *of){

    fprintf(of,"<pnml xmlns=\"http://www.pnml.org/version-2009/grammar/pnml\">\n");
    fprintf(of,"\t<net id=\"n1\" type=\"http://www.pnml.org/version-2009/grammar/ptnet\">\n");
    fprintf(of,"\t\t<page id=\"main-page\">\n");
}

void epilog(FILE *of,char* name){

    fprintf(of,"\t\t</page>\n");
    fprintf(of,"\t\t<name>\n");
    fprintf(of,"\t\t\t<text>%s</text>\n",name);
    fprintf(of,"\t\t</name>\n");
    fprintf(of,"\t</net>\n");
    fprintf(of,"</pnml>\n");

}

     //Graphical elements, except position, are constant
void generate_place(FILE *of,int id,char* name,int x_pos,int y_pos,int marking){
    fprintf(of,"\t\t\t<place id=\"ras-%1d\">\n",id);
    fprintf(of,"\t\t\t\t<name>\n");
    fprintf(of,"\t\t\t\t\t<graphics>\n");
    fprintf(of,"\t\t\t\t\t\t<offset x=\"30\" y=\"0\"/>\n");
    fprintf(of,"\t\t\t\t\t</graphics>\n");
    fprintf(of,"\t\t\t\t\t<text>%s</text>\n",name);
    fprintf(of,"\t\t\t\t</name>\n");

    if(marking != 0){
        fprintf(of,"\t\t\t\t<initialMarking>\n");
        fprintf(of,"\t\t\t\t\t<graphics>\n");
        fprintf(of,"\t\t\t\t\t\t<offset x=\"0\" y=\"0\"/>\n");
        fprintf(of,"\t\t\t\t\t</graphics>\n");
        fprintf(of,"\t\t\t\t\t\t<text>%1d</text>\n",marking);
        fprintf(of,"\t\t\t\t</initialMarking>\n");
    }
    fprintf(of,"\t\t\t\t<graphics>\n");
    fprintf(of,"\t\t\t\t\t<position x=\"%1d\" y=\"%1d\"/>\n",x_pos,y_pos);
    fprintf(of,"\t\t\t\t\t<dimension x=\"20\" y=\"20\"/>\n");
    fprintf(of,"\t\t\t\t\t<fill color=\"rgb(255,255,255)\"/>\n");
    fprintf(of,"\t\t\t\t\t<line color=\"rgb(0,0,0)\"/>\n");
    fprintf(of,"\t\t\t\t</graphics>\n");
    fprintf(of,"\t\t\t</place>\n");
}

void generate_transition(FILE *of,int id,char* name,int x_pos,int y_pos){
    fprintf(of,"\t\t\t<transition id=\"ras-%1d\">\n",id);
    fprintf(of,"\t\t\t\t<name>\n");
    fprintf(of,"\t\t\t\t\t<graphics>\n");
    fprintf(of,"\t\t\t\t\t\t<offset x=\"30\" y=\"0\"/>\n"); //name inside
    fprintf(of,"\t\t\t\t\t</graphics>\n");
    fprintf(of,"\t\t\t\t\t<text>%s</text>\n",name);
    fprintf(of,"\t\t\t\t</name>\n");

    fprintf(of,"\t\t\t\t<graphics>\n");
    fprintf(of,"\t\t\t\t\t<position x=\"%1d\" y=\"%1d\"/>\n",x_pos,y_pos);
    fprintf(of,"\t\t\t\t\t<dimension x=\"24\" y=\"16\"/>\n");
    fprintf(of,"\t\t\t\t\t<fill color=\"rgb(255,255,255)\"/>\n");
    fprintf(of,"\t\t\t\t\t<line color=\"rgb(0,0,0)\"/>\n");
    fprintf(of,"\t\t\t\t</graphics>\n");
    fprintf(of,"\t\t\t</transition>\n");
}

void generate_arc(FILE *of,int id,int source,int target){/*forget id parameter*/

    fprintf(of,"\t\t\t<arc id=\"ras-%1d\" source=\"ras-%1d\" target=\"ras-%1d\">\n",arc_id,source,target);
    fprintf(of,"\t\t\t\t<inscription>\n");
    fprintf(of,"\t\t\t\t\t<text>1</text>\n");
    fprintf(of,"\t\t\t\t</inscription>\n");

    fprintf(of,"\t\t\t\t<graphics>\n");
    fprintf(of,"\t\t\t\t\t<line color=\"rgb(0,0,0)\" style=\"solid\"/>\n");
    fprintf(of,"\t\t\t\t</graphics>\n");
    fprintf(of,"\t\t\t</arc>\n");

    arc_id++;
}

void generate_even_process(FILE *of,int process_index,int x_ini,int y_ini,int n,int first_id,
                int res_marking,int am_i_the_last){
    int id_offset = 0;
    int x;
    int y;
    int p_id=0,t_id=0,r_id=0;
    int i;
    char prefix[100];

    x = x_ini;y = y_ini;

    for(i=0;i<n;i++){
        t_id = first_id+id_offset;
        sprintf(prefix,"t_%1d_%1d",process_index,i);
        generate_transition(of,t_id,prefix,x,y);
        id_offset++;
        /*before creating next state and resource places, create arcs to previous state and resource places*/
        if(i!=0){
            generate_arc(of,first_id+id_offset,p_id,t_id);
            generate_arc(of,first_id+id_offset,t_id,r_id);
        }

        y = y+element_offset;
        p_id = first_id+id_offset;
        sprintf(prefix,"p_%1d_%1d",process_index,i);
        generate_place(of,p_id,prefix,x,y,0);
        id_offset++;
        r_id = first_id+id_offset;

        sprintf(prefix,"r_%1d_%1d",process_index,i);
        generate_place(of,r_id,prefix,x+element_offset,y,res_marking); //resource place
        id_offset++;
        y = y+element_offset;

        generate_arc(of,first_id+id_offset,t_id,p_id);
        generate_arc(of,first_id+id_offset,r_id,t_id);

        if(!am_i_the_last){ /*arcs from the resource to the next process transitions*/
            generate_arc(of,first_id+id_offset,r_id+process_index_offset-2,r_id);
            generate_arc(of,first_id+id_offset,r_id,r_id+process_index_offset+1);
        }
    }
    /*last transition and related arcs*/
    t_id = first_id+id_offset;
    sprintf(prefix,"t_%1d_%1d",process_index,n);
    generate_transition(of,t_id,prefix,x,y);
    generate_arc(of,first_id+id_offset,p_id,t_id);
    generate_arc(of,first_id+id_offset,t_id,r_id);
}

void generate_odd_process(FILE *of,int process_index,int x_ini,int y_ini,int n,int first_id,
                int res_marking,int am_i_the_last){
    int id_offset = 0;
    int x;
    int y;
    int p_id=0,t_id=0,r_id=0;
    char prefix[100];
    int i;

    x = x_ini;y = y_ini;

    for(i=0;i<n;i++){
        t_id = first_id+id_offset;
        sprintf(prefix,"t_%1d_%1d",process_index,i);
        generate_transition(of,t_id,prefix,x,y);
        id_offset++;
        /*before creating next state and resource places, create arcs to previous state and resource places*/
        if(i!=0){
            generate_arc(of,first_id+id_offset,t_id,p_id);
            generate_arc(of,first_id+id_offset,r_id,t_id);
        }

        y = y+element_offset;
        p_id = first_id+id_offset;
        sprintf(prefix,"p_%1d_%1d",process_index,i);
        generate_place(of,p_id,prefix,x,y,0);
        id_offset++;
        r_id = first_id+id_offset;
        sprintf(prefix,"r_%1d_%1d",process_index,i);
        generate_place(of,r_id,prefix,x+element_offset,y,res_marking); //resource place
        id_offset++;
        y = y+element_offset;

        generate_arc(of,first_id+id_offset,p_id,t_id);
        generate_arc(of,first_id+id_offset,t_id,r_id);

        /*arcs from the resource to the next process transitions*/
        if(!am_i_the_last){ //arcs from the resource to the next process transitions
            generate_arc(of,first_id+id_offset,r_id,r_id+process_index_offset-2);
            generate_arc(of,first_id+id_offset,r_id+process_index_offset+1,r_id);
        }
    }
    /*last transition and related arcs*/
    t_id = first_id+id_offset;
    sprintf(prefix,"t_%1d_%1d",process_index,n);
    generate_transition(of,t_id,prefix,x,y);
    generate_arc(of,first_id+id_offset,t_id,p_id);
    generate_arc(of,first_id+id_offset,r_id,t_id);

}

void generate_model(FILE *of,int how_many,int how_deep,int res_marking){
    int x_ini = 100,
        y_ini = 100;
    int i;

    for(i=0;i<how_many;i++){
        if((i % 2) == 0){
            generate_even_process(of,i,x_ini+i*2*element_offset,y_ini,how_deep,
                    process_index_offset*(i+1),res_marking,i==how_many-1);
        }else{
            generate_odd_process(of,i,x_ini+i*2*element_offset,y_ini,how_deep,
                    process_index_offset*(i+1),res_marking,i==how_many-1);
        }
    }
}

void error(char *m){
	perror(m);
	exit(1);
}


/*
   Modify for the proper invocation

   model_generator <number_processes> <process_deeper> <resource initial place marking> <output pnml-file>

*/

int main(int argc,char *argv[]) {
    int number_processes = 0,
        process_deeper = 0,
        m_0_res = 0;
    char epilog_text[255];
    FILE *of;



	if(argc < 4){
		error("Invocation:\n\tmodel_generator <number_proc> <process_deeper> <initial_res_marking> <file>\n");
	}
	number_processes = atoi(argv[1]);
	process_deeper = atoi(argv[2]);
	m_0_res = atoi(argv[3]);
	of = fopen(argv[4],"w");

	/* Dealing with invocation parameters coreection */
	if(of == NULL)
		error("I am having problems with the output file");
	if(number_processes<2)
		error("At least 2 processes are required");
	if(process_deeper<1)
		error("Process deeper must be of at leats 1 state");
	if(m_0_res<1)
		error("Resource initial marking must be positive");

	/* Parameters are correct: go */
    preface(of);
    generate_model(of,number_processes,process_deeper,m_0_res);

    sprintf(epilog_text,"RAS_%1d_%1d_%1d",number_processes,process_deeper,m_0_res);
    epilog(of,epilog_text);


    return 0;
}
