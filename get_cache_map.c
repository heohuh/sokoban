void get_cache_map()        // 캐시에서 이전 맵 정보를 가져온다. 
{
	int i = 0;

	if( cache_size > 4 )    //캐시 사이즈가 4보다 커도의미없으므로 최댓값은 4로 유지.
	{
		cache_size = 4;
	}

	if( cache_size < 0 )  //캐시가 비어있다면 가져올 정보가 없으므로 패스한다.
	{
		cache_size = -1;
		return;
	}

	for( i = 0; i < MAXLINE; i++ )  //가장 최신 맵 정보를 현재 맵으로 가져온다.
	{
		strcpy( cur_map[i], cache_map[ cache_size ][i] );
	}
	get_cur_coordinate();   // 캐릭터의 위치를 새로 업데이트한다.
	cache_size--;       //캐시의 크기를 줄인다. 
}
