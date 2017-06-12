#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>

#define MAXWIDTH 31
#define MAXLINE 31
#define N 11

void map_load();			// 맵 파일에서 맵을 불러옴. - 허진영 
void save_map( int level );		// sokoban 파일에 저장함. - 박석태
int load_map();				// 저장된 파일에서 읽어들임. - 김도균
void get_cur_coordinate();		// @ 의 위치를 찾아줌. - 윤동우
int get_box_coordinate( int level );	// 상자의 위치를 찾아줌 - 윤동우 
int get_gold_coordinate();		// 금의 위치를 찾아줌. - 윤동우
void set_coordinate( int x, int y );	// 캐릭터의 이동을 그려줌- 허진영 
int check_clear(int level);		// 게임을 클리어 했는지 확인. - 윤동우
int getch(void);			// 에코 없이 한글자를 읽어들임. - 윤동우
void get_cur_map( int level );		// 현재 맵의 정보를 얻어옴 - 김도균
void set_cache_map();			// 5개까지 캐시맵에 업데이트함 - 김도균 
void get_cache_map();			// 캐시에서 이전 정보를 가져옴 - 김도균 
void init_cache_map();			// 맵을 캐시할 배열을 초기화함 - 김도균
void error_check( int n, int m );	// 박스와 금의 갯수가 다르면 에러를 띄움 - 허진영
void print_help();			// 헬프 메뉴를 보여줌 - 박석태
void print_map( char ch );		// 맵 정보를 화면에 출력함 - 김도균
void insert_userName();			// 사용자 이름을 저장 - 박석태
void setElapsedTime( int level );	// 각 레벨별로 경과된 시간을 구해 줌 - 김도균
void save_rank();			// 시간별로 순위를 파일에 저장 - 윤동우
void print_top( char ch, int level );	// top 랭커들을 화면에 출력 - 김도균 

int box_count;				// 박스의 갯수를 저장
int cache_size = -1;			// 캐시의 크기를 나타낸다

char map[5][MAXLINE][MAXWIDTH];		// 전체 맵 정보
char cur_map[MAXLINE][MAXWIDTH];	// 현재 맵 정보
char cache_map[5][MAXLINE][MAXWIDTH];	// 캐시 맵 정보

int cur_coord[2];			// 현재 캐릭터의 위치 정보
int box_coord[31][2];			// 현재 박스의 위치 정보
int gold_coord[31][2];			// 현재 금의 위치 정보

char cur_user_name[N];			// 현재 플레이 하고 있는 유저
time_t cur_start_time;			// 시작시간
time_t cur_end_time;			// 끝시간

int  user_count[5];			// 랭킹에 등록된 사용자의 수
char all_user_name[5][6][N];		// 랭킹에 등록된 사용자의 이름
float all_clear_time[5][6];		// 랭킹에 등록된 시간


                              
void setElapsedTime( int level )  //플레이한 시간 구해준다.
{
  int i = 0, j = 0;
  float dif = 0;
  float tmp = 0;
  char  tmp_name[N] = {0,};

                                                 
  dif = difftime( cur_end_time, cur_start_time ); //시간차를 구하는 변수

                                                                    
  strcpy( all_user_name[level][user_count[level]], cur_user_name ); //유저의 이름과 시간을 기록한다.
  all_clear_time[level][ user_count[level] ] = dif;

                                            
  for( i = 0; i < user_count[level]; i++ )//오름차순으로 정렬한다.
  {       
    for( j = 0; j < user_count[level]; j++ )
   	{
      if( all_clear_time[level][j] > all_clear_time[level][j+1] ) 
	  {
        tmp = all_clear_time[level][j];
        all_clear_time[level][j] = all_clear_time[level][j+1];
        all_clear_time[level][j+1] = tmp;

        strcpy( tmp_name, all_user_name[level][j] );
        strcpy( all_user_name[level][j], all_user_name[level][j+1] );
        strcpy( all_user_name[level][j+1], tmp_name );

      }
    }
  }
}

                 
void save_rank()              //랭킹 정보를 저장한다.
{
  int i = 0, j = 0, k = 0;
  FILE *fd = NULL;

  fd = fopen( "ranking.txt", "w" );

                         
  for( i = 0; i <= 4; i++) //레벨별로 플레이한 사용자의 정보를 기록한다
  {
    if( user_count[i] < 0 ) {
      break;
    } else {
      fprintf(fd, "level %d\n", i);
    }
    if( user_count[i] > 4 ) { user_count[i] = 4; }
    for( j = 0; j <= user_count[i]; j++ ) {
      fprintf(fd, "rank %s ", all_user_name[i][j] );
      fprintf(fd, "%.1lf\n", all_clear_time[i][j] );
    }
    fprintf(fd, "\n");
  }

  fprintf(fd, "%s\n", "end");
  fclose( fd );
}

               
void load_rank()            //랭킹 파일을 읽어들여 로드한다.
{
  FILE *fd         = NULL;
  int level        = 0;
  int i            = 0;
  int j            = 0;

  char tag[MAXWIDTH]     = {0,};
  char name[MAXWIDTH]     = {0,};

                                     
  fd = fopen( "ranking.txt", "r" );       //랭킹 파일을 오픈하고 없으면 읽어들이지 않는다.
  if( fd == NULL ) {
    return ;
  }

  do {

                                             
    for( i = 0; i < MAXWIDTH; i++ ) {           //태그 배열과 이름 배열 초기화
      tag[i] = 0;
    }
    for( i = 0; i < MAXWIDTH; i++ ) {
      name[i] = 0;
    }
    fscanf( fd, "%s ", tag );		// 읽어들여서...

    if( strstr( tag, "level") ) {	// 레벨이면 레벨정보를 기록
      fscanf( fd, "%d\n", &level);
      j = 0;
    }else if( strstr( tag, "end") ) {	// 엔드이면 파일의 끝이니 루프를 종료
      break;
    }else if( strstr( tag, "rank") ) {	// 랭크이면 사용자 정보가 들어 있으니 읽어들인다
      user_count[level]++;
      fscanf( fd, "%s %f\n", all_user_name[level][j], &all_clear_time[level][j] );
      j++;
    }
  }while( 1 );

  fclose( fd );
}

                           
void init_user_count()       //사용자의 카운트를 증가시킨다.
{
  int i = 0;
  for( i = 0; i < 5; i++ ) {
    user_count[i] = -1;			// 사용자가 없으면 -1로 초기화
  }
}

                                       
void print_top( char ch, int level )   //현재 랭킹정보를 화면에 출력한다.
{
  int i = 0, j = 0;

  system("clear");
  printf("  Hello %s \n\n\n", cur_user_name );

                              
  user_count[level]--;          //레벨별로 사용자 정보를 전부 출력한다.
  for( i = 0; i < 5; i++ ) {
    printf("map %d\n", i+1 );
    for( j = 0; j <= user_count[i]; j++ ) {
      printf("%s %.1lfsec \n",  all_user_name[i][j], all_clear_time[i][j] );
    }
  }

  printf("(Command) %c \n", ch );
  getch();
}

void map_load()    //맵파일로부터 맵을 읽어들이는 함수 - 허진영
{
  FILE *fd         = NULL;
  int level        = -1;
  int line         = 0;
  int i            = 0;

  char ch          = 0;
  char tag[MAXWIDTH]     = {0,};

  fd = fopen( "map.txt", "r" );
  if( fd == NULL ) {
    printf("map not exist \n" );
    return;
  }

  do {
    for( i = 0; i < MAXWIDTH; i++ )//태그배열 초기화
   	{  
      tag[i] = 0;
    }
    i = 0;
    do {
      fscanf( fd, "%c", &ch );
      tag[i] = ch;
      i++;
    }while( ch  != '\n' );

    if( strstr( tag, "map") ) //맵 문자열과 일치하면 맵을 시작한다
   	{
      level++;
      line = 0;
      continue;
    } else if( strstr( tag, "end" ) ) //end문자열과 일치하면 더이상 읽지 않는다.
   	{
      break;
    } else {
      strcpy( map[level][line], tag );  //읽어들인 맵을 배열에 저장한다.
      line++;
    }
  } while( 1 );

  fclose( fd );
}

void save_map( int level )  //현재 게임중인 맵을 sokoban파일에 저장하는 함수 - 박석태
{
  int i = 0;
  FILE *fd = NULL;

  fd = fopen( "sokoban.txt", "w" );  

  fprintf(fd, "level\n");
  fprintf(fd, "%d\n", level );   //나중에 읽어들이기 위해 레벨과 함께 저장한다.
  fprintf(fd, "\n" );
  fprintf(fd, "map\n");

  for( i = 0; i < MAXLINE; i++ ) {
    fprintf(fd, "%s", cur_map[i] );  //현재 진행중인 맵을 파일에 저장한다.
  }
  fclose( fd ); 
}

int load_map()  // 저장된 파일에서 이전에 실행했던 레벨과 맵을 읽어들이는 함수 - 김도균
{
  int i = 0;
  int j = 0;
  int level = 0;
  char ch = 0;
  char tag[MAXWIDTH] = {0,};

  FILE *fd = NULL;

  fd = fopen("sokoban.txt", "r");

  if( fd == NULL ) {
    return -1;
  }

  fscanf( fd, "%s\n", tag );
  if( !strcmp( tag, "level" ) )   //이전에 플레이한 레벨을 로드한다.
  {
    fscanf( fd, "%d\n", &level );
    printf("level: %d \n", level );
  }

  for( i = 0; i < MAXLINE; i++ ) {
    for( j = 0; j < MAXWIDTH; j++ ) {
      cur_map[i][j] = 0;         //현재 진행중인 맵 정보를 초기화한다.
    }
  }

  for( i = 0; i < MAXWIDTH; i++ ) {
    tag[i] = 0;
  }
  i = 0;
  do {
    fscanf( fd, "%c", &ch );
    tag[i] = ch;
    i++;
  }while( ch  != '\n' );

  i = 0; j = 0;
  if( strstr( tag, "map" ) ) {
    while( fscanf( fd, "%c", &ch ) != EOF ) //이전에 실행했던 맵 정보를 읽어들인다.
   	{
      cur_map[i][j] = ch;
      if( ch != '\n' ) {
        j++;
      }else {
        i++;
        j = 0;
      }
    }
  }

  return level;
}

void get_cur_coordinate()  //플레이어('@')의 위치를 찾는 함수 - 윤동우
{
  int i = 0;
  int j = 0;

  for( i = 0; i < MAXLINE; i++ )  {
    for( j = 0; j < MAXWIDTH; j++ ) {
      if( cur_map[i][j] == '@' ) { cur_coord[0] = i; cur_coord[1] = j; break;}   
    }
  }
}   //전체 배열을 다 돌면서 '@'문자 정보의 위치를 저장한다.

int get_box_coordinate( int level )  //상자의 위치를 찾는 함수 - 윤동우
{
  int i = 0;
  int j = 0;
  box_count = 0;

  for( i = 0; i < MAXLINE; i++) {
    for( j = 0; j < MAXLINE; j++) {
      if( map[level][i][j] == 'O' ) {
        box_coord[box_count][0] = i;
        box_coord[box_count][1] = j;
        box_count++;
      }
    }
  }            //전체 배열을 다 돌면서 박스 정보를 저장한다.
  return box_count;
}

int get_gold_coordinate()  // 금의 위치를 찾는 함수-윤동우
{
  int i = 0;
  int j = 0;
  int gold_count = 0;   

  for( i = 0; i < MAXLINE; i++) {
    for( j = 0; j < MAXLINE; j++) {
      if( cur_map[i][j] == '$' ) {
        gold_coord[gold_count][0] = i;
        gold_coord[gold_count][1] = j;
        gold_count++;
      }
    }
  }                  //전체 배열을 다 돌면서 금의 정보를 저장한다.
  return gold_count;
}

void set_coordinate( int x, int y )  //캐릭터가 이동할 때마다 현재 맵에 새로 그려주는 함수 - 허진영
{
  int i = 0;
  int j = 0;

  int new_x = x, old_x = cur_coord[0];  //이전 위치 정보를 저장한다.
  int new_y = y, old_y = cur_coord[1];

  if( cur_map[x][y] != '#' && cur_map[x][y] != '$') //이동할때 벽이나 금이 없다면 캐릭터를 이동시킨다.
  {

    set_cache_map();         //맵정보를 업데이트 하기전에 미리 캐시한다.

    cur_map[old_x][old_y] = ' ';  //캐릭터를 이동시킨다.
    cur_map[new_x][new_y] = '@';

    cur_coord[0] = new_x; //새로 이동한 좌표를 업데이트 한다.
    cur_coord[1] = new_y;

  } else if ( cur_map[x][y] == '$' ) //이동할 위치에 금이 있다면 함께 이동한다.
  {
    if( new_x < old_x ) {

      if( cur_map[x-1][y] != '$' && cur_map[x-1][y] != '#' ) //위쪽 방향으로 이동중이었다면
	  {
        set_cache_map();
        cur_map[new_x-1][new_y] = '$';  //금을 위로 한 칸 이동
        cur_map[new_x][new_y] = '@';
        cur_map[old_x][old_y] = ' ';
        cur_coord[0] = new_x;
        cur_coord[1] = new_y;
      }

    }else if( new_x > old_x )//아랫쪽 방향을 이동중이었다면
   	{
      if( cur_map[x+1][y] != '$' && cur_map[x+1][y] != '#' ) {
        set_cache_map();
        cur_map[new_x+1][new_y] = '$';   //금을 아래로 한 칸 이동
        cur_map[new_x][new_y] = '@';
        cur_map[old_x][old_y] = ' ';
        cur_coord[0] = new_x;
        cur_coord[1] = new_y;
      }

    }else if( new_y < old_y ) //왼쪽 방향으로 이동중이었다면
   	{
      if( cur_map[x][y-1] != '$' && cur_map[x][y-1] != '#' ) {
        set_cache_map();
        cur_map[new_x][new_y-1] = '$';             //금을 왼쪽으로 한 칸 이동
        cur_map[new_x][new_y] = '@';
        cur_map[old_x][old_y] = ' ';
        cur_coord[0] = new_x;
        cur_coord[1] = new_y;
      }

    }else if( new_y > old_y ) //오른쪽 방향으로 이동중이었다면
	{
      if( cur_map[x][y+1] != '$' && cur_map[x][y+1] != '#' ) {
        set_cache_map();
        cur_map[new_x][new_y+1] = '$';  //금을 오른쪽으로 한 칸 이동.
        cur_map[new_x][new_y] = '@';
        cur_map[old_x][old_y] = ' ';
        cur_coord[0] = new_x;
        cur_coord[1] = new_y;
      }
    }
  }

  for( i = 0; i < box_count; i++ )  // 박스를 새로 그려준다.
  {
    if (cur_map[ box_coord[i][0] ][ box_coord[i][1] ] != '@' &&
        cur_map[ box_coord[i][0] ][ box_coord[i][1] ] != '$') {
      cur_map[ box_coord[i][0] ][ box_coord[i][1] ] = 'O';
    }
  }
}

int check_clear(int level)   // 모든 박스의 위치에 금이 있다면 게임을 클리어하는 함수 - 윤동우
{
  int i = 0;

  for( i = 0; i < box_count; i++ ) {
    if( cur_map[ box_coord[i][0] ][ box_coord[i][1] ] != '$' ) { return -1; }
  }
  return 0;
}

int getch(void)
{
  int ch;
  struct termios buf;
  struct termios save;
  tcgetattr(0,&save);
  buf = save;
  buf.c_lflag&=~(ICANON|ECHO);
  buf.c_cc[VMIN] = 1;
  buf.c_cc[VTIME] = 0;
  tcsetattr(0,TCSAFLUSH,&buf);
  ch = getchar();
  tcsetattr(0,TCSAFLUSH,&save);
  return ch;
}

void get_cur_map( int level )   //현재 맵의 정보를 얻어온다 - 김도균
{
  int i = 0;
  int j = 0;

  for( i = 0; i < MAXLINE; i++ ) //맵을 초기화한다.
  {
    for( j = 0; j < MAXWIDTH; j++ ) {
      cur_map[i][j] = 0;
    }
  }

  for( i = 0; i < MAXLINE; i++ ) //레벨에 맞는 맵을 가져온다.
  {
    strcpy( cur_map[i], map[level][i] );
  }
}

void set_cache_map()   //최대 5개까지 캐시맵에 업데이트 한다.(최신순으로) undo 구현 - 김도균
{
  int i = 0;
  int j = 0;

  cache_size++;

  if( cache_size < 5 ) //캐시 크기가 5보다 작으면 캐시에 저장한다.
  {
    for( i = 0; i < MAXLINE; i++ ) {
      strcpy( cache_map[cache_size][i], cur_map[i] );
    }
  } else //캐시 크기가 5라면 더 이상 저장할 곳이 없으므로 캐시를 비워준다.
  {
    cache_size = 4;
    for( i = 0; i < cache_size; i++ ) {
      for( j = 0; j < MAXLINE; j++ )  {
        strcpy( cache_map[i][j], cache_map[i+1][j] );
      }
    }

    for( i = 0; i < MAXLINE; i++ ) //캐시를 비운뒤 맵 정보를 저장한다.
	{
      strcpy( cache_map[cache_size][i], cur_map[i] );
    }
  }
}

void get_cache_map()  // 캐시에서 이전 맵 정보를 가져온다. - 김도균
{
  int i = 0;

  if( cache_size > 4 ) //캐시 사이즈가 4보다 커도 의미없으므로 최댓값은 4로 유지한다.
  {
    cache_size = 4;
  }

  if( cache_size < 0 ) //캐시가 비어있다면 가져올 정보가 없으므로 패스한다.
  {
    cache_size = -1;
    return;
  }

  for( i = 0; i < MAXLINE; i++ ) //가장 최신 맵 정보를 현재 맵으로 가져온다.
  {
    strcpy( cur_map[i], cache_map[ cache_size ][i] );
  }
  get_cur_coordinate();  //캐릭터의 위치를 새로 업데이트한다.
  cache_size--;   //캐시의 크기를 줄인다.
}

void init_cache_map()  //맵을 캐시할 배열을 초기화한다. -김도균
{
  int i = 0;
  int j = 0;
  int k = 0;

  for( i = 0; i < 5; i++ ) {
    for( j = 0; j < MAXLINE; j++ ) {
      for( k = 0; k < MAXWIDTH; k++ ) {
        cache_map[i][j][k] = 0;
      }
    }
  }
}

void error_check( int n, int m )  //박스의 갯수와 금의 갯수가 다르면 에러를 띄운다. - 허진영
{
  if( n != m ) {
    printf("map error...a number of '$' and '0' are not equal\n");
    exit(0);
  }
}

void print_help()  // 헬프 메뉴를 보여준다. - 박석태
{
  system("clear");
  printf("h(왼쪽), j(아래), k(위), l(오른쪽)  \n");
  printf("u(undo) \n");
  printf("r(replay) \n");
  printf("n(new) \n");
  printf("e(exit) \n");
  printf("s(save) \n");
  printf("f(file upload) \n");
  printf("d(display help) \n");
  printf("t(top) \n");
  printf("\n");
}

void print_map( char ch )   //맵 정보를 화면에 출력해준다. -김도균
{
  int i = 0;

  printf("  Hello %s \n\n\n", cur_user_name );

  if( ch != 'd' ) {
    for( i = 0; i < MAXLINE; i++ ) {
      printf("%s", cur_map[i] );
    }
  } else {
    print_help();
  }

  if( ch != 'h' && ch != 'j' && ch != 'k' && ch != 'l' ) {
    printf("(Command) %c \n", ch);
  }else {
    printf("(Command) \n");
  }
}

void insert_userName()  //사용자 이름을 저장한다. -박석태
{
  printf("input your name: ");
  scanf("%s", cur_user_name );
}

int main()
{
  int i = 0;
  int level = 0;
  int ret = 0;
  int box_cnt = 0;
  int gold_cnt = 0;
  char choice = 0;

  char ch = 0;

  printf("Start...\n");
  init_user_count();

  map_load();
  load_rank();
  get_cur_map( level );
  get_cur_coordinate();

  box_cnt = get_box_coordinate( level );
  gold_cnt = get_gold_coordinate();
  error_check( box_cnt, gold_cnt );

  insert_userName();
  user_count[level]++;

  system("clear");

  print_map( 0 );
  time( &cur_start_time );
  while( ch = getch() ) {

    if( ch == 'h' ) {
      set_coordinate( cur_coord[0], cur_coord[1]-1);
    }else if ( ch == 'j' ) {
      set_coordinate( cur_coord[0]+1, cur_coord[1]);
    }else if ( ch == 'k' ) {
      set_coordinate( cur_coord[0]-1, cur_coord[1]);
    }else if ( ch == 'l' ) {
      set_coordinate( cur_coord[0], cur_coord[1]+1);
    }else if( ch == 'u' ) {
      get_cache_map();
      system("clear");
      print_map(ch);
      continue;
    }else if( ch == 'r' ) {
      get_cur_map( level );
      get_cur_coordinate( level ) ;
      cache_size = -1;
      system("clear");
      print_map( ch );
      continue;
    }else if( ch == 'n' ) {
      level = 0;
      cache_size = -1;
      get_cur_map( level );
      get_cur_coordinate( level );
      system("clear");
      print_map( ch );
      continue;
    }else if( ch == 'd' ) {
      print_map( ch );
      continue;
    }else if( ch == 'e' ) {
      time( &cur_end_time );
      setElapsedTime( level );
      save_map( level );
      save_rank();
      system("clear");
      print_map( ch );
      exit(0);
    } else if( ch == 's' ) {
      save_map( level );
      system("clear");
      print_map( ch );
      continue;
    } else if( ch == 'f' ) {
      level = load_map();
      get_cur_coordinate();
      get_box_coordinate( level );
      cache_size = -1;
      system("clear");
      print_map( ch );
      continue;
    } else if( ch == 't' ) {
      system("clear");
      print_top( ch, level );
      system("clear");
      print_map( 0 );
      continue;
    }else {
      system("clear");
      print_map(0);
      continue;
    }

    system("clear");
    print_map( ch );

    if( level < 4 && !check_clear(level) ) {
      time( &cur_end_time );
      setElapsedTime( level );
      level++;
      cache_size = -1;
      user_count[level]++;

      get_cur_map( level );
      get_cur_coordinate();
      box_cnt = get_box_coordinate( level );
      gold_cnt = get_gold_coordinate();
      error_check( box_cnt, gold_cnt );
      set_cache_map();

      printf("Level %d Clear!!! \n", level );
      printf("Press any key Go to next level");

      time( &cur_start_time );

    } else if( level == 4 && !check_clear(level) ) {
      time( &cur_end_time );
      setElapsedTime( level );
      save_rank();

      printf("congratulation!!! ");
      exit(0);
    }
  }

  return 0;
}

