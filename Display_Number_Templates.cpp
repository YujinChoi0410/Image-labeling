#define TmpW	20	//template 영상의 너비는 20
#define TmpH	40	//template 영상의 높이는 40
BYTE CharTemplate[10][TmpH][TmpW]; //0~9 template을 담는 3차원 행렬[10][40][20]

void Read_Template()
{
	int i, j, x, y, num, w2 = TmpW, h2 = TmpH;
	FILE *fp;

	fp = fopen("c:\\Temp.dat", "r");
	for (i = 0; i < 10; i++) {
		fscanf(fp, "%d\n", &num);
		for (y = 0; y < h2; y++) {
			for (x = 0; x < w2; x++) fscanf(fp, "%d,", &CharTemplate[i][y][x]);
			fscanf(fp, "\n");
		}
	}
	fclose(fp);

}

void CHomework::OnClickedTemplates()
{
	BYTE **img;
	int i, x, y, w = TmpW, h = TmpH, x0, y0;

	Read_Template(); //template 읽어오기

	img = cmatrix(h, w); //영상 할당하기

	x0 = 10; //template 영상을 display할 초기 위치는 (10,10)
	y0 = 10;
	for (i = 0; i < 10; i++) {
		for (y = 0; y < h; y++) {
			for (x = 0; x < w; x++) {
				if (CharTemplate[i][y][x] == 0) img[y][x] = 0; //template 픽셀값이 0이면 0(검은색)
				else img[y][x] = 255; //0이 아니면 255 (흰색)
			}
		}
		DisplayCimage2D(img, w, h, x0, y0, FALSE); //영상 display하기
		x0 += w + 10; //template 영상 간의 간격은 가로 10
	}
}
