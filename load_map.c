int load_map()  // 저장된 파일에서 이전에 실행했떤 레벨과 맵을 읽어들이는 함수
{
	int i = 0;
	int j = 0;
	int level = 0;
	char ch = 0;
	char tag[MAXWIDTH] = {0,};

	FILE *fd = NULL;

	fd = fopen("sokoban.txt", "r");

	if( fd == NULL ) 
	{
		return -1;
	}

	fscanf( fd, "%s\n", tag );
	if(!strcmp( tag, "level" ))             //이전에 플레이하던 레벨을 로드 
	{
		fscanf( fd, "%d\n", &level );
		printf("level: %d \n", level );
	}

	for( i = 0; i < MAXLINE; i++ )
	{
		for( j = 0; j < MAXWIDTH; j++ )
		{
			cur_map[i][j] = 0;            //현재 진행중인 맵 정보를 초기화한다. 
		}
	}

	i = 0; j = 0;
	fscanf( fd, "%s\n", tag );
	if( !strcmp( tag, "map" ) ) {
		while( fscanf( fd, "%c", &ch ) != EOF )   //이전에 실행했던 맵 정보를 읽어들임.
		{
			cur_map[i][j] = ch;
			if( ch != '\n' ) 
			{
				j++;
			}
			else {
				i++;
				j = 0;
			}
		}
	}
	return level;
}
