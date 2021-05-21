void Character_Matching(BYTE **img, int w, int h, int *Num, float *Score)
{
	//w, h�� �Է¿��� ����

	//���� ���� �����ϱ�
	BYTE **Normal, **img2;
	int i, x, y, cnt;
	float max; //�ִ� matching score�� ����

	img2 = cmatrix(TmpH, TmpW); //template ���� �޸� �Ҵ��ϱ�
	Normal = cmatrix(TmpH, TmpW); //Normalized ���� �޸� �Ҵ��ϱ�

	Scale_Norm(img, Normal, w, h); //���� ũ�� 20x40���� normalization

	max = 0;
	for (i = 0; i < 10; i++) {
		cnt = 0; //��ġ�ϴ� �ȼ� �� �ʱ�ȭ
		for (y = 0; y < TmpH; y++) {
			for (x = 0; x < TmpW; x++) {
				if (CharTemplate[i][y][x] == 0) img2[y][x] = 0; //template �ȼ����� 0�̸� 0(������)
				else img2[y][x] = 255; //0�� �ƴϸ� 255 (���)

				if (Normal[y][x] == img2[y][x]) cnt++; //��ġ�ϴ� �ȼ��� ������ counting
			}
		}
		*Score = (float)cnt / 800; //marching score: �ش� ������ Ȯ��

		if (*Score > max) {// ���� ū matching score�� �ش� ���� ã��
			max = *Score;
			*Num = i;
		}
	}
	*Score = max; // ���� matching score

	//�޸� �Ҵ� �����ϱ�
	free_cmatrix(Normal, TmpH, TmpW);
	free_cmatrix(img2, TmpH, TmpW);
}

void Numeric_Recognition(BYTE **Oimg, int **Label, int width, int height, int *Area, int Num, int m_Ref, int x0, int y0, int y1)
{
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetMainWnd();
	CChildFrame *pChildFrame = (CChildFrame *)pMainFrame->MDIGetActive(NULL);
	CImageProcessingView *pView = (CImageProcessingView *)pChildFrame->GetActiveView();
	CDC *dc = pView->GetDC();

	int i, k, x, y, w, h, yy0, xx0, Result, yy, max;
	RECT Rc;
	BYTE **img;
	float Score; //matching score
	char msg[128]; //text�� display�ϱ� ���� �迭

	i = 0;
	xx0 = x0; //xx0: �νĵ� ���� display�� x��ǥ ��ġ
	yy0 = y0 + height + 5; //yy0: �νĵ� ���� display�� y��ǥ ��ġ
	max = 0;
	//RcNum = 0;

	for (k = 1; k <= Num; k++) { //Num: �ִ� label ��
		if (Area[k] >= m_Ref) { //label���� ���ִ� ȭ���� ������ 100���̻��̸�
			Find_Region(Label, width, height, k, &Rc); //x,y��ǥ�� �ִ�, �ּҰ� ã�Ƽ� Rc�� ����
			w = Rc.right - Rc.left + 1; //display�� ������ ���� ����
			h = Rc.bottom - Rc.top + 1; //display�� ������ ���� ����
			if (h < 50 && h > w) { //���α���<50 �̸鼭 ���α���>���α��� �̸�
				img = cmatrix(h, w); //display�� ���� �޸� �Ҵ��ϱ�

				//Label���� k�� ���� ���� �����
				for (y = 0; y < h; y++) { //Label���� ��ĵ�ϱ�
					for (x = 0; x < w; x++) {
						if (Label[y + Rc.top][x + Rc.left] == k) img[y][x] = 255; //Label���� k�϶� ��ü�����̹Ƿ� ���
						else img[y][x] = 0; //��ü���� �ƴϸ� ��Ӱ�
					}
				}

				Character_Matching(img, w, h, &Result, &Score); //�Է¿��� img�� �ִ� matching score���
																//Result: 0~9 �� matching score ���� ū ����

				if (Score >= 0.75) { //score�� 0.75�̻��� �͸� display
					//Rlist[RcNum].left = Rc.left;
					//Rlist[RcNum].right = Rc.right;
					//Rlist[RcNum].top = Rc.top;
					//Rlist[RcNum++].bottom = Rc.bottom;

					sprintf(msg, "%d", Result); //Result: �νĵ� ���� 
					yy = y1 + (int)((Rc.bottom + Rc.top)*0.5); //�Է¿��� �������� display�� y��ǥ ��ġ 
					dc->TextOut(x0 + Rc.left, yy, msg, strlen(msg)); //�Է¿��� �������� �νĵ� ���� ����ϱ�. 

					DisplayCimage2D(img, w, h, xx0, yy0, FALSE); //�νĵ� ���ڿ��� display�ϱ�
					sprintf(msg, "%d: %5.2f", Result, Score); //Result:  �νĵ� ����, matching score: �ν�Ȯ��
					dc->TextOut(xx0 + w + 5, yy0, msg, strlen(msg)); //����, matching score display�ϱ�

					//�νĵ� ���� ������� ���ĸ��缭 display�ϱ�
					i++;
					if (h > max) max = h; //max: �� �� ���� �ִ� ����� �� ���� �� ���α��� 
					if (i >= 4) { //0��°���� 3��°����, �� 4������ �� �ٿ� ���
						i = 0;
						yy0 += max + 10; //�����ٷ� ��ġ �ѱ��
						xx0 = x0;
						max = 0;
					}
					else { //������ ���� ���� 110 �ȼ�
						xx0 += 110;
					}
				}
				free_cmatrix(img, h, w); //�޸� �Ҵ� ����
			}
		}
	}

	pView->ReleaseDC(dc);
}

void CHomework::OnClickedLpr2()
{
	int x, y, x0, y0, width, height, **Label, *Area, Num;
	BYTE **Oimg, **Oimg1, **Oimg2; //Oimg: �Է¿���, Oimg1: thresholding�� �Է¿���, Oimg2: reverse�� �Է¿���

	if (!GetCurrentImageInfo(&width, &height, &x0, &y0)) return; //�Է¿������� �б�

	Oimg = cmatrix(height, width); //�Է¿��� �޸� �Ҵ��ϱ�
	GetCurrentImageGray(Oimg); //�Է¿������� �б�

	Oimg1 = cmatrix(height, width); //��¿��� �޸� �Ҵ��ϱ�(reverse �� ����)
	Oimg2 = cmatrix(height, width); //��¿��� �޸� �Ҵ��ϱ�(reverse �� ����)

	//�Է¿����� 136 �������� thresholding(reverse ��) 
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			if (Oimg[y][x] >= 136) Oimg1[y][x] = 255; //�ȼ����� 136 �̻��̸� ���
			else Oimg1[y][x] = 0; //�� �ܿ��� ������
		}
	}

	//�Է¿��� reverse��Ű��
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			Oimg2[y][x] = 255 - Oimg[y][x];
		}
	}

	//�Է¿����� 136 �������� thresholding(reverse ��)
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			if (Oimg2[y][x] >= 136) Oimg2[y][x] = 255; //�ȼ����� 136 �̻��̸� ���
			else Oimg2[y][x] = 0; //�� �ܿ��� ������
		}
	}

	Label = imatrix(height, width); //Label ���� �Ҵ��ϱ�
	Area = (int *)malloc(width*height * sizeof(int)); //area��(�׷�ȭ�� �ȼ� ����) ������ �޸� �Ҵ��ϱ�

	//reverse ��
	Image_Labeling(Oimg1, width, height, Label, Area, &Num); //image labeling �����Ͽ� ��ü ���� �����ϱ�
	Make_Noise_Removed_Image(Oimg1, width, height, Label, Area, Num, 100); //label������ noise�����ϱ�
	DisplayCimage2D(Oimg1, width, height, x0 + width + 10, y0, FALSE); //noise ���ŵ� ���� display�ϱ�
	Read_Template(); //templates �о����
	Numeric_Recognition(Oimg1, Label, width, height, Area, Num, 100, x0, y0, y0); //Num: �ִ� Label ��, Area: ȭ�Ұ� ���ִ� ����

	//reverse ��
	Image_Labeling(Oimg2, width, height, Label, Area, &Num); //image labeling �����Ͽ� ��ü ���� �����ϱ�
	Make_Noise_Removed_Image(Oimg2, width, height, Label, Area, Num, 100); //label������ noise�����ϱ�
	DisplayCimage2D(Oimg2, width, height, x0 + width + 10, y0 + height + 10, FALSE); //noise ���ŵ� ���� display�ϱ�
	Read_Template(); //templates �о����
	Numeric_Recognition(Oimg2, Label, width, height, Area, Num, 100, x0, y0 + 250, y0); //Num: �ִ� Label ��, Area: ȭ�Ұ� ���ִ� ����

	//�޸� �Ҵ� �����ϱ�																
	free(Area);
	free_imatrix(Label, height, width);
	free_cmatrix(Oimg, height, width);
	free_cmatrix(Oimg1, height, width);
	free_cmatrix(Oimg2, height, width);
}
