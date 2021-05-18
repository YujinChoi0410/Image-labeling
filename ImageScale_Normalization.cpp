void Scale_Norm(BYTE **ImageGray, BYTE **OutputGray, int nW, int nH)
{
	int x, y;
	int nOutW = 20, nOutH = 40;	// nW, nH: �������� ���� ����
								// nOutW, nOutH: ��ȯ�� ������ ���� ����

	for (y = 0; y < nOutH; y++)
		for (x = 0; x < nOutW; x++)
		{
			OutputGray[y][x] = ImageGray[y*(nH / nOutH)][x*(nW / nOutW)];
		}
}