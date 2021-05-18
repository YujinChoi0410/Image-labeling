void CHomework::OnClickedMatching()
{
	//���� ���� �����ϱ�
	int x0, y0, width, height, Num, i, x, y, w = TmpW, h = TmpH;
	BYTE **img1, **Normal, **img2, **Match_Image;

	if (!GetCurrentImageInfo(&width, &height, &x0, &y0)) return; //�Է¿������� �б�

	img1 = cmatrix(height, width); //�Է¿��� �޸� �Ҵ��ϱ�
	GetCurrentImageGray(img1); //�Է¿������� �б�

	Normal = cmatrix(h, w); //Normalized ���� �޸� �Ҵ��ϱ�

	Scale_Norm(img1, Normal, width, height); //���� ũ�� 20x40���� normalization

	x0 = x0 + width + 10; //normalized ������ display�� ��ġ
	DisplayCimage2D(Normal, w, h, x0, y0, true); //normalized ���� display�ϱ�

	/////

	img2 = cmatrix(h, w); //template(0~9) ���� �Ҵ��ϱ�
	Match_Image = cmatrix(h, w); //matching ��� ���� �Ҵ��ϱ�

	x0 = x0 + w + 10; //template ������ display�� ��ġ
	for (i = 0; i < 10; i++) {
		for (y = 0; y < h; y++) {
			for (x = 0; x < w; x++) {
				if (CharTemplate[i][y][x] == 0) img2[y][x] = 0; //template �ȼ����� 0�̸� 0(������)
				else img2[y][x] = 255; //0�� �ƴϸ� 255 (���)

				if (Normal[y][x] == img2[y][x]) Match_Image[y][x] = 0; //�� ������ �ȼ����� ������ 0(������)
				else Match_Image[y][x] = 255;	//�� ������ �ȼ����� �ٸ��� 255(���)
			}
		}
		DisplayCimage2D(img2, w, h, x0, y0, FALSE); // template ���� display�ϱ�
		DisplayCimage2D(Match_Image, w, h, x0 + w + 10, y0, FALSE); // matching ���� display�ϱ�
		y0 += h + 10; //template ���� ���� ������ ���� 10
	}

	//���� �޸� �Ҵ� �����ϱ�
	free_cmatrix(img1, height, width);
	free_cmatrix(Normal, h, w);
	free_cmatrix(img2, h, w);
	free_cmatrix(Match_Image, h, w);
}