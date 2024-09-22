void GetFilePathToken( const char *pFilePath, char *pDrive, char *pPath, char *pFileName, char *pExt )
{
	int naNextIndex[MAX_PATH];
	//char *pStr;
	char cTemp[MAX_PATH];
	char *pToken = NULL;
	char caSep[] = "\\";

	strcpy( cTemp, pFilePath );

	pToken = strtok( cTemp, caSep );
	naNextIndex[0] = strlen(pToken)+1;

	int c = 0;
	while( pToken != NULL )
	{
		//pStr = pToken;
		pToken = strtok( 0, caSep );
		if( pToken != NULL )
		{
			c++;
            naNextIndex[c] = naNextIndex[c-1] + strlen(pToken) + 1;
		}
	}

	if( pDrive != NULL )
		strcpy( pDrive, cTemp );

	if( pFileName != NULL )
		strcpy( pFileName, &cTemp[ naNextIndex[c-1] ] );

}
