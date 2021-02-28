#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node{
  int data[6][6];
  struct Node *prev;
  struct Node *next;
}Node;

Node* front;
Node* newNode(int **map){
  Node* node = (Node*)malloc(sizeof(Node));
  for(int i=0; i<6; i++){
    for(int k=0; k<6; k++){
      node->data[i][k]= map[i][k];
    }
  }
  node->prev=NULL;
  node->next = NULL;
  return node;
}

Node* list_end(Node* front){
  Node* tmp = front;
  while(tmp->next!=NULL){
    tmp=tmp->next;
  }
  return tmp;
}
void addNode(Node* prev_new, Node* next_new){
  prev_new->next = next_new;
  next_new->prev = prev_new;
}
int case_check(Node* front, int** check){
  Node *tmp= front;
  int sig=1;
  while(tmp!=NULL&& sig==1){
    sig=0;
    for(int i=0; i<6; i++){
      for(int k=0; k<6; k++){
        
       if(tmp->data[i][k]!=check[i][k]){
        sig=1;
        break;
       } 
      }
      if(sig==1) break;
    }
    tmp= tmp->next;
  }
  
  return sig;
}

int num_of_list(Node* list){
  int count=0;
  while(list!=NULL){
    count++;
    list=list->next;
  }
  return count;
}

typedef enum {
	start,
	left,
	right,
	up,
	down,
	quit,
	N_op
} commands ;

typedef enum {
	vertical,
	horizontal
} direction ;

typedef struct {
	int id ;
	int y1, y2 ;	// y1: the minimum of y, y2: the maximum of y
	int x1, x2 ;	// x1: the minimum of x, x2: the maximum of x
	int span ;		// the number of cells
	direction dir ;	// the direction of the car
} car_t ;

char * op_str[N_op] = {
	"start",
	"left",
	"right",
	"up",
	"down",
	"quit"
} ;

int n_cars = 0 ;
car_t * cars = 0x0;
int cells[6][6] ; 
// cells[Y][X]
// A6 -> cells[5][0]
// F4 -> cells[3][5]
// F1 -> cells[0][5]

/* --- NOFIX */
commands 
get_op_code (char * s)
{
  if(strcmp(s,"start")==0)
    return 0;
  else if(strcmp(s,"left")==0)
    return 1;
  else if(strcmp(s,"right")==0)
    return 2;
  else if(strcmp(s,"up")==0)
    return 3;
  else if(strcmp(s,"down")==0)
    return 4;
  else if(strcmp(s,"quit")==0)
    return 5;
  else
    return -1;

}

int
load_game (char * filename)
{
	char buf[128];
	int i=0;
	FILE * fp = fopen(filename,"r");
  if(fp==NULL){
    printf("Wrong File Name\n");
    return 0;
  }

  fgets(buf,sizeof(buf),fp);
	
  int num = atoi(buf);
  n_cars = num;
 
  cars = malloc(sizeof(car_t)*(num+1));
  
	while(!feof(fp)){

		fgets(buf,sizeof(buf),fp);
    char *ptr = strtok(buf, ":"); 
    
    while (ptr != NULL)               
    {
        int car_num = i/3;
        int tok_n= i%3;
        cars[car_num].id = car_num;
        if(tok_n==0){
          
          char v = ptr[0]-'A';
          ptr=ptr+1;
          int h = atoi(ptr);
         
          cars[car_num].x1=v;
          cars[car_num].y1 = 6-h;

        }
        else if(tok_n==1){
          if(!strcmp(ptr,"horizontal")){
            cars[car_num].dir=horizontal;
          }
          else{
            cars[car_num].dir = vertical;
          }
        }     
        else {
          int length = atoi(ptr);
          cars[car_num].span = length;
          if(cars[car_num].dir==1){
            cars[car_num].x2=cars[car_num].x1+cars[car_num].span-1;
            cars[car_num].y2= cars[car_num].y1;
          }
          else{
            cars[car_num].x2 = cars[car_num].x1;
            cars[car_num].y2 = cars[car_num].y1+cars[car_num].span-1;
          }

        }
        ptr = strtok(NULL, ":");
        i++;
    }
	}
	fclose(fp);
  return 0;
}

void
display ()
{
  for(int i=0; i<6; i++){
    for(int k=0; k<6; k++){
      if(cells[i][k]==0)
        printf("%c ",'+');
      else 
        printf("%d ", cells[i][k]);
    }
    printf("\n");
  }

}

int
update_cells ()
{
	memset(cells, 0, sizeof(int) * 36) ; // clear cells before the write.
  int number_of_cars = n_cars;

  for(int i=0 ; i<number_of_cars; i++ ){
    if(cars[i].x2>5 || cars[i].x1<0 || cars[i].y1<0 || cars[i].y2>5){
      printf("Error: out of range.\n");
      return 1;
    }
    if(cars[i].dir==0){
          for(int k=cars[i].y1; k<=cars[i].y2; k++)
            cells[k][cars[i].x1] = i+1;
        }
    else{
        for(int k=cars[i].x1; k<=cars[i].x2; k++)
          cells[cars[i].y1][k] = i+1;
      }
    }
  if(cells[2][5]==1){
    printf("\n성공\n");
    return 0;
  }
    return 0;
}

int
move (int id, int op)
{
  int new_id=id-1;
  switch(op){
    case 1 :
      if(cars[new_id].dir==0||cars[new_id].x1-1 <0){
         printf("에러: 이동할 수 없음.zz %d %d\n",id, op);
        return -1;
      }
      if(cells[cars[new_id].y1][cars[new_id].x1-1]!=0){
        printf("Error: Another car is parking.\n");
        return -1;
      }
      cars[new_id].x1--; 
      cars[new_id].x2--;
      break;
    case 2 :
       if(cars[new_id].dir==0||cars[new_id].x2+1 >5){
        printf("에러: 이동할 수 없음.zz %d %d\n",id, op);
        return -1;
      }
      if(cells[cars[new_id].y1][cars[new_id].x2+1]!=0){
        
        printf("Error: Another car is parking.zz\n");
        return -1;
      }
      cars[new_id].x1++; 
      cars[new_id].x2++;
      break;
    case 3 :
        if(cars[new_id].dir==1||cars[new_id].y1-1 <0){
         printf("에러: 이동할 수 없음.zz %d %d\n",id, op);
        return -1;
      }
      if(cells[cars[new_id].y1-1][cars[new_id].x2]!=0){
        printf("Error: Another car is parking.\n");
        
      }
      cars[new_id].y1--; 
      cars[new_id].y2--;
      break;
    case 4 :
    if(cars[new_id].dir==1||cars[new_id].y2+1>5){
         printf("에러: 이동할 수 없음.zz %d %d\n",id, op);
        return -1;
      }
      if(cells[cars[new_id].y2+1][cars[new_id].x2]!=0){
        printf("Error: Another car is parking.\n");
        return -1;
      }
      cars[new_id].y1++; 
      cars[new_id].y2++;
  }
  return 0;
}

void m_set(int **arr){
  for(int i=0; i<6; i++){
    for(int k=0; k<6; k++)
      arr[i][k]=cells[i][k];
  }
}

int path_serach(int new_id){
  int path=0, count=0;
  int  **arr;
  int id;
  arr = (int**) malloc ( sizeof(int*) * 6 );
  for(int i=0; i<6; i++)
    arr[i] = (int*) malloc ( sizeof(int) * 6 );

  id=new_id-1;
  if(cars[id].dir==0){
    int y_up = cells[cars[id].y1-1][cars[id].x1];
    int y_down = cells[cars[id].y2+1][cars[id].x1];

    if(cars[id].y1-1>=0 && y_up==0){
      path =3;
      move(new_id,path);
      update_cells();
      m_set(arr);
      if(case_check(front,arr)==1)
        return path;
      move(new_id,4);
      update_cells();
    }
    if(cars[id].y2+1<=5 && y_down==0){
      path = 4;
      move(new_id,path);
      update_cells();
      m_set(arr);
      if(case_check(front,arr)==1)
        return path;
      move(new_id,3);
      update_cells();
    }
  }
  else{
    int x_left = cells[cars[id].y1][cars[id].x1-1];
    int x_right = cells[cars[id].y2][cars[id].x2+1];
    if(cars[id].x2+1<=5 && x_right == 0){
      path =2;
      move(new_id,path);
      update_cells();
      m_set(arr);
      if(case_check(front,arr)==1)
        return path;
      move(new_id,1);
      update_cells();
    }
    if(cars[id].x1-1>=0 && x_left==0){
      path =1;
      move(new_id,path);
      update_cells();
      m_set(arr);
      if(case_check(front,arr)==1)
        return path;
      move(new_id,2);
      update_cells();
    }
  }
  return 0;
}


int
main ()
{
	char buf[128] ;
	int op ;
	int id ;
  int ** arr;
  arr = (int**) malloc ( sizeof(int*) * 6 );
  for(int i=0; i<6; i++){
    arr[i] = (int*) malloc ( sizeof(int) * 6 );
  }

	load_game("board1.txt");
  update_cells();
  m_set(arr);
  display();
  front = newNode(arr);
  printf("\n");
  int count=0;
  while(cells[2][5]!=1){
    
    for(int i=1; i<=n_cars; i++){
      
      int path = path_serach(i);
      if(path!=0){
        
        m_set(arr);
        printf("\n");
      
        display();
        printf("%s  %d\n",op_str[path],i);
        printf("%d\n",count); count++;
        if(cells[2][5]==1) break;
        addNode(list_end(front), newNode(arr));
        break;
      
      }
    }
  
  }
  
	return 0;
}
 
