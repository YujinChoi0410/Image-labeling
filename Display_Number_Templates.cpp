#define TmpW	20	//template ������ �ʺ�� 20
#define TmpH	40	//template ������ ���̴� 40
BYTE CharTemplate[10][TmpH][TmpW]; //0~9 template�� ��� 3���� ���[10][40][20]

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

	Read_Template(); //template �о����

	img = cmatrix(h, w); //���� �Ҵ��ϱ�

	x0 = 10; //template ������ display�� �ʱ� ��ġ�� (10,10)
	y0 = 10;
	for (i = 0; i < 10; i++) {
		for (y = 0; y < h; y++) {
			for (x = 0; x < w; x++) {
				if (CharTemplate[i][y][x] == 0) img[y][x] = 0; //template �ȼ����� 0�̸� 0(������)
				else img[y][x] = 255; //0�� �ƴϸ� 255 (���)
			}
		}
		DisplayCimage2D(img, w, h, x0, y0, FALSE); //���� display�ϱ�
		x0 += w + 10; //template ���� ���� ������ ���� 10
	}
}
