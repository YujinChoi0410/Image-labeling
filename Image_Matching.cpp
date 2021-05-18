void CHomework::OnClickedMatching()
{
	//각종 변수 선언하기
	int x0, y0, width, height, Num, i, x, y, w = TmpW, h = TmpH;
	BYTE **img1, **Normal, **img2, **Match_Image;

	if (!GetCurrentImageInfo(&width, &height, &x0, &y0)) return; //입력영상정보 읽기

	img1 = cmatrix(height, width); //입력영상 메모리 할당하기
	GetCurrentImageGray(img1); //입력영상정보 읽기

	Normal = cmatrix(h, w); //Normalized 영상 메모리 할당하기

	Scale_Norm(img1, Normal, width, height); //영상 크기 20x40으로 normalization

	x0 = x0 + width + 10; //normalized 영상을 display할 위치
	DisplayCimage2D(Normal, w, h, x0, y0, true); //normalized 영상 display하기

	/////

	img2 = cmatrix(h, w); //template(0~9) 영상 할당하기
	Match_Image = cmatrix(h, w); //matching 결과 영상 할당하기

	x0 = x0 + w + 10; //template 영상을 display할 위치
	for (i = 0; i < 10; i++) {
		for (y = 0; y < h; y++) {
			for (x = 0; x < w; x++) {
				if (CharTemplate[i][y][x] == 0) img2[y][x] = 0; //template 픽셀값이 0이면 0(검은색)
				else img2[y][x] = 255; //0이 아니면 255 (흰색)

				if (Normal[y][x] == img2[y][x]) Match_Image[y][x] = 0; //두 영상의 픽셀값이 같으면 0(검은색)
				else Match_Image[y][x] = 255;	//두 영상의 픽셀값이 다르면 255(흰색)
			}
		}
		DisplayCimage2D(img2, w, h, x0, y0, FALSE); // template 영상 display하기
		DisplayCimage2D(Match_Image, w, h, x0 + w + 10, y0, FALSE); // matching 영상 display하기
		y0 += h + 10; //template 영상 간의 간격은 가로 10
	}

	//영상 메모리 할당 해제하기
	free_cmatrix(img1, height, width);
	free_cmatrix(Normal, h, w);
	free_cmatrix(img2, h, w);
	free_cmatrix(Match_Image, h, w);
}