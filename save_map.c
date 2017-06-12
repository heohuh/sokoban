void save_map(int level)    //현재 게임중인 맵을 sokoban파일에 저장하는 함수
{
	int i = 0;
	FILE *fd = NULL;

	fd = fopen("sokoban.txt", "w");

	fprintf(fd, "level\n");
	fprintf(fd, "%d\n", level);           // 나중에 읽어들이기 위해서 레벨과 함께 저장 
	fprintf(fd, "\n" );
	fprintf(fd, "map\n");

	for( i = 0; i < MAXLINE; i++ )
	{
		fprintf(fd, "%s", cur_map[i] );    //현재 진행중인 맵을 파일에 저장 
	}
	fclose( fd );
}
