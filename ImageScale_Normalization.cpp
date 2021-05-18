void Scale_Norm(BYTE **ImageGray, BYTE **OutputGray, int nW, int nH)
{
	int x, y;
	int nOutW = 20, nOutH = 40;	// nW, nH: 원영상의 폭과 높이
								// nOutW, nOutH: 변환된 영상의 폭과 높이

	for (y = 0; y < nOutH; y++)
		for (x = 0; x < nOutW; x++)
		{
			OutputGray[y][x] = ImageGray[y*(nH / nOutH)][x*(nW / nOutW)];
		}
}