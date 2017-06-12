void map_load()   //맵 파일로부터 맵 읽어들이는 함수 - 허진영
{
	FILE *fd = NULL;
	int level = -1;
	int line = 0;
	int i = 0;

	char ch = 0;
	char tag[MAXWIDTH] = {0,};

	fd = fopen("map.txt", "r");

	do 
	{
		for( i = 0; i < MAXWIDTH; i++ )   //tag배열 초기화
		{
			tag[i] = 0;
		}

		i = 0;
		do 
		{
			fscanf(fd, "%c", &ch);
			tag[i] = ch;
			i++;
		} while( ch  != '\n' );            //맵 파일로부터 한 라인씩 읽어들인다. 

		if( !strcmp( tag, "map\r\n"))      //"map" 문자열과 일치하면 맵의 시작.
		{
			level++;
			line = 0;
			continue;
		}
		else if(!strcmp(tag,"end\r\n"))  //"end" 문자열과 일치하면 더 이상 읽지 않음 
		{
			break;
		} 
		else 
		{
			strcpy(map[level][line], tag);     //읽어들인 맵을 배열에 저장
			line++;
		}
	} while(1);

	fclose( fd );
}




void set_coordinate( int x, int y ) //캐릭터가 이동할 때마다 현재 맵에 새로 그려주는 함수. - 허진영
{
	int i = 0;
	int j = 0;

	int new_x = x, old_x = cur_coord[0];    // 이전 위치 정보를 저장한다. 
	int new_y = y, old_y = cur_coord[1];

	if( cur_map[x][y] != '#' && cur_map[x][y] != '$')  //이동 할때 벽이나 금이 없다면 캐릭터를 이동시킨다.
	{
		set_cache_map();  //맵 정보를 업데이트 하기전에 미리 캐시한다.

		cur_map[old_x][old_y] = ' '; //캐릭터를 이동시킨다.
		cur_map[new_x][new_y] = '@';

		cur_coord[0] = new_x;  //새로 이동한 좌표를 업데이트한다.
		cur_coord[1] = new_y;

	} 
	else if ( cur_map[x][y] == '$' )  //이동할 위치에 금이 있다면 함께 이동한다.
	{
		if( new_x < old_x ) 
		{

			if( cur_map[x-1][y] != '$' && cur_map[x-1][y] != '#' ) //위쪽 방향으로 이동중이었다면
			{
				set_cache_map();
				cur_map[new_x-1][new_y] = '$';     //금을 위로 한 칸 이동.
				cur_map[new_x][new_y] = '@';
				cur_map[old_x][old_y] = ' ';
				cur_coord[0] = new_x;
				cur_coord[1] = new_y;
			}

		}
		else if( new_x > old_x )  //아랫쪽 방향으로 이동중이었다면
		{
			if( cur_map[x+1][y] != '$' && cur_map[x+1][y] != '#' )
			{
				set_cache_map();
				cur_map[new_x+1][new_y] = '$';    //금을 아래로 한 칸 이동.
				cur_map[new_x][new_y] = '@';
				cur_map[old_x][old_y] = ' ';
				cur_coord[0] = new_x;
				cur_coord[1] = new_y;
			}

		}
		else if( new_y < old_y )      //왼쪽 방향으로 이동중이었다면
		{
			if( cur_map[x][y-1] != '$' && cur_map[x][y-1] != '#' )
			{
				set_cache_map();
				cur_map[new_x][new_y-1] = '$';  // 금을 왼쪽으로 한 칸 이동.
				cur_map[new_x][new_y] = '@';
				cur_map[old_x][old_y] = ' ';
				cur_coord[0] = new_x;
				cur_coord[1] = new_y;
			}

		}
		else if( new_y > old_y )   //오른쪽 방향으로 이동중이었다면 
		{
			if( cur_map[x][y+1] != '$' && cur_map[x][y+1] != '#' )
			{
				set_cache_map();
				cur_map[new_x][new_y+1] = '$';   //금을 오른쪽으로 한 칸 이동.
				cur_map[new_x][new_y] = '@';
				cur_map[old_x][old_y] = ' ';
				cur_coord[0] = new_x;
				cur_coord[1] = new_y;
			}
		}
	}

	for( i = 0; i < box_count; i++ )   //박스를 새로 그려준다.
	{
		if (cur_map[ box_coord[i][0] ][ box_coord[i][1] ] != '@' && cur_map[ box_coord[i][0] ][ box_coord[i][1] ] != '$') 
		{
			cur_map[ box_coord[i][0] ][ box_coord[i][1] ] = 'O';
		}
	}
}


void error_check( int n, int m )        //박스의 갯수와 금의 갯수가 다르면 에러를 띄운다. - 허진영
{
	if( n != m ) {
		printf("map error...a number of '$' and 'O'is not equal.. \n");
		exit(0);
	}
}
