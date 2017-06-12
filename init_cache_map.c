void init_cache_map()   //맵을 캐시할 배열을 초기화한다
{
	int i = 0;
	int j = 0;
	int k = 0;

	for( i = 0; i < 5; i++ ) 
	{
		for( j = 0; j < MAXLINE; j++ ) 
		{
			for( k = 0; k < MAXWIDTH; k++ ) 
			{
				cache_map[i][j][k] = 0;
			}
		}
	}
}
