void Image_Labeling(BYTE **img1, int width, int height, int **Label, int *Area, int *Num)
{
	int k, x, y, *r, num, left, top;

	r = (int*)malloc(width * height * sizeof(int)); //r table�� ������ ũ�⸸ŭ �Ҵ��ϱ�

	//Label���� �ʱ�ȭ
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			if (img1[y][x] > 128) Label[y][x] = 1; //��ü �ȼ��� �ʱⰪ�� ���� 1�� ����
			else Label[y][x] = 0;  //��� �ȼ� �ʱⰪ�� 0���� ���� 
		}
	}

	//Label������ boundary�� 0���� �ʱ�ȭ. boundary�κ��� ��ü�� �� �����Ƿ�
	for (x = 0; x < width; x++) {
		Label[0][x] = 0;		//���� ���� �� �� �ȼ��� 0����
		Label[height - 1][x] = 0; //���� ������ �� �� �ȼ��� 0����
	}

	for (y = 0; y < height; y++) {
		Label[y][0] = 0;		//���� ���� �� �� �ȼ��� 0����
		Label[y][width - 1] = 0;  //���� �Ʒ��� �� �� �ȼ��� 0����
	}

	num = 0; //label ��
	for (y = 1; y < height - 1; y++) { //�ڳ� �ȼ��� ��, �� �ȼ��� �����Ƿ� (1,1)���� ����
		for (x = 1; x < width - 1; x++) {
			if (Label[y][x] > 0) {  //��ü�� �ȼ��� ���� 
				left = Label[y][x - 1];  //���� �ȼ� �о����
				top = Label[y - 1][x];   //���� �ȼ� �о����

				if (left == 0 && top == 0) { //generate new region pixel
					num++;	//label �� �߰�
					r[num] = num;	//r table �߰�
					Label[y][x] = num; //label �� �־��ֱ�
					Area[num] = 1; //�� label���� ���� area(�ȼ�����) �߰�
				}

				else if (left == 0 && top != 0) { //copy label from above
					Label[y][x] = r[top]; //�� �ȼ� �� ��������
					Area[top]++; //�� �ȼ� label���� ���� area ����++
				}

				else if (left != 0 && top == 0) { //copy lebel from the left
					Label[y][x] = r[left]; //���� �ȼ� �� ��������
					Area[left]++; //���� �ȼ� label���� ���� area ����++
				}

				else if (left != 0 && top != 0) { //��, �� �ȼ����� ��� 0�� �ƴ� ��
					//�� ��쿣 r���� ���ϱ�
					if (r[left] > r[top]) { //�����ȼ� r�� > �����ȼ� r��
						Label[y][x] = r[top]; //���� �ȼ� r�� ��������
						r[left] = r[top]; //���� �ȼ� r���� ���� ���ֱ�
						Area[r[top]]++;
					}
					else { //�����ȼ� r�� < �����ȼ� r��
						Label[y][x] = r[left]; //���� �ȼ� r�� ��������
						r[top] = r[left]; //���� �ȼ� r���� ���� ���ֱ�
						Area[r[left]]++;
					}
				}

			}
		}
	}

	*Num = num; //���� ū label ��

	//���� �׷쿡 ���� �ȼ��� ���� label�� ������ ������ֱ�
	for (k = 1; k <= num; k++) {
		if (k != r[k]) {
			r[k] = r[r[k]];
			Area[r[k]] += Area[k];
			Area[k] = 0;
		}
	}

	//��ü �ȼ��� ���� label���� r������ ��ü�ϱ�
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			if (Label[y][x] > 0) Label[y][x] = r[Label[y][x]];
		}
	}

	free(r); //r tabel �Ҵ� �����ϱ�

}

void Make_Noise_Removed_Image(BYTE **img1, int width, int height, int **Label, int *Area, int  Num, int m_LabelNum)
{
	int k, x, y;

	//noise ���ŵ� ���� ������ img1�̴�.
	//noise ���ŵ� ���� ���� �ȼ��� ��� 0���� �ʱ�ȭ�ϱ�
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			img1[y][x] = 0;
		}
	}

	//m_LabelNum �̻��� �ȼ������� �׷츸 thresholding
	for (k = 1; k <= Num; k++) { //Num: �ִ� label��
		if (Area[k] >= m_LabelNum) {
			for (y = 0; y < height; y++) {
				for (x = 0; x < width; x++) {
					if (Label[y][x] == k) img1[y][x] = 255;
				}
			}
		}
	}
}

static void Find_Region(int **Label, int width, int height, int k, RECT *Rc)
{
	int x, y;

	//�ʱ�ȭ
	Rc->top = 9999;
	Rc->bottom = 0;
	Rc->left = 9999;
	Rc->right = 0;

	//��ü label ���� ��ĵ
	//��ü���� x, y��ǥ�� �ִ�, �ּҰ� ã��
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			if (Label[y][x] == k) { //label ���� k�� ��
				if (y < Rc->top) Rc->top = y;
				if (y > Rc->bottom) Rc->bottom = y;
				if (x < Rc->left) Rc->left = x;
				if (x > Rc->right) Rc->right = x;
			}
		}
	}
}

void Display_Labeled_Images(int **Label, int width, int height, int *Area, int Num, int m_Ref, int x0, int y0)
{
	int k, x, y, w, h, yy0;
	RECT Rc; //��ü ������ ���� structure
	BYTE **img; //display�� ����

	yy0 = y0;
	for (k = 1; k <= Num; k++) {
		if (Area[k] >= m_Ref) { //�ȼ��� �����ִ� ������ m_Ref�̻��� �� 

			//Label���󿡼� label���� k�� ���� x,y��ǥ�� �ִ�, �ּҰ��� Rc structure��(��������)����.
			Find_Region(Label, width, height, k, &Rc);

			//display�� ���� ����
			w = Rc.right - Rc.left + 1; //���α���
			h = Rc.bottom - Rc.top + 1; //���α���
			img = cmatrix(h, w); //display ���� �Ҵ��ϱ�

			//display���� �� �������ֱ�
			for (y = 0; y < h; y++) {
				for (x = 0; x < w; x++) {
					if (Label[y + Rc.top][x + Rc.left] == k) img[y][x] = 255;
					else img[y][x] = 0;
				}
			}
			DisplayCimage2D(img, w, h, x0, yy0, TRUE); //��ü���� display�ϱ�
			yy0 += h + 10; //������ ������ 10�ȼ�
			free_cmatrix(img, h, w); //���� �Ҵ� �����ϱ�
		}
	}

}

void CHomework::OnClickedLabeling()
{
	//���� ���� �����ϱ�
	int x0, y0, width, height, **Label, *Area, Num;
	BYTE **img1;

	if (!GetCurrentImageInfo(&width, &height, &x0, &y0)) return; //�Է¿������� �б�

	img1 = cmatrix(height, width); //�Է¿��� �޸� �Ҵ��ϱ�
	GetCurrentImageGray(img1); //�Է¿������� �б�

	Label = imatrix(height, width); //label���� �޸� �Ҵ��ϱ�
	Area = (int *)malloc(width*height * sizeof(int)); //area��(�׷�ȭ�� �ȼ� ����) ������ �޸� �Ҵ��ϱ�

	Image_Labeling(img1, width, height, Label, Area, &Num);//label���� �����ϱ�

	UpdateData(TRUE); //���� �ֱٿ� ������Ʈ�� ������ �����´�.

	Make_Noise_Removed_Image(img1, width, height, Label, Area, Num, m_LabelNum);//label������ noise�����ϱ�

	DisplayIimage2D(Label, width, height, x0, y0 + height, true); //label ���� display�ϱ�
	DisplayCimage2D(img1, width, height, x0, y0 + height * 2, true); //noise ���ŵ� ���� display�ϱ�

	Display_Labeled_Images(Label, width, height, Area, Num, m_LabelNum, x0 + width + 10, y0); //�󺧺��� ��ü ���� display�ϱ�

	//�޸� �Ҵ�����
	free(Area); //area��(�׷�ȭ�� �ȼ� ����) ������ �޸� �Ҵ� �����ϱ�
	free_imatrix(Label, height, width);	//label���� �޸� �Ҵ� �����ϱ�
	free_cmatrix(img1, height, width); //noise���ſ��� �޸� �Ҵ� �����ϱ�
}
