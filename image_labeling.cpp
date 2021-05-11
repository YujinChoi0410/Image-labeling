void Image_Labeling(BYTE **img1, int width, int height, int **Label, int *Area, int *Num)
{
	int k, x, y, *r, num, left, top;

	r = (int*)malloc(width * height * sizeof(int)); //r table을 영상의 크기만큼 할당하기

	//Label영상 초기화
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			if (img1[y][x] > 128) Label[y][x] = 1; //물체 픽셀의 초기값을 전부 1로 설정
			else Label[y][x] = 0;  //배경 픽셀 초기값은 0으로 설정 
		}
	}

	//Label영상의 boundary를 0으로 초기화. boundary부분은 물체일 수 없으므로
	for (x = 0; x < width; x++) {
		Label[0][x] = 0;		//가장 왼쪽 한 줄 픽셀들 0으로
		Label[height - 1][x] = 0; //가장 오른쪽 한 줄 픽셀들 0으로
	}

	for (y = 0; y < height; y++) {
		Label[y][0] = 0;		//가장 위쪽 한 줄 픽셀들 0으로
		Label[y][width - 1] = 0;  //가장 아래쪽 한 줄 픽셀들 0으로
	}

	num = 0; //label 값
	for (y = 1; y < height - 1; y++) { //코너 픽셀은 좌, 상에 픽셀이 없으므로 (1,1)부터 시작
		for (x = 1; x < width - 1; x++) {
			if (Label[y][x] > 0) {  //물체인 픽셀에 대해 
				left = Label[y][x - 1];  //왼쪽 픽셀 읽어오기
				top = Label[y - 1][x];   //위쪽 픽셀 읽어오기

				if (left == 0 && top == 0) { //generate new region pixel
					num++;	//label 값 추가
					r[num] = num;	//r table 추가
					Label[y][x] = num; //label 값 넣어주기
					Area[num] = 1; //새 label값에 대한 area(픽셀개수) 추가
				}

				else if (left == 0 && top != 0) { //copy label from above
					Label[y][x] = r[top]; //위 픽셀 값 가져오기
					Area[top]++; //위 픽셀 label값에 대한 area 개수++
				}

				else if (left != 0 && top == 0) { //copy lebel from the left
					Label[y][x] = r[left]; //왼쪽 픽셀 값 가져오기
					Area[left]++; //왼쪽 픽셀 label값에 대한 area 개수++
				}

				else if (left != 0 && top != 0) { //좌, 상 픽셀값이 모두 0이 아닐 때
					//이 경우엔 r값을 비교하기
					if (r[left] > r[top]) { //왼쪽픽셀 r값 > 위쪽픽셀 r값
						Label[y][x] = r[top]; //위쪽 픽셀 r값 가져오기
						r[left] = r[top]; //왼쪽 픽셀 r값도 같게 해주기
						Area[r[top]]++;
					}
					else { //왼쪽픽셀 r값 < 위쪽픽셀 r값
						Label[y][x] = r[left]; //왼쪽 픽셀 r값 가져오기
						r[top] = r[left]; //위쪽 픽셀 r값도 같게 해주기
						Area[r[left]]++;
					}
				}

			}
		}
	}

	*Num = num; //가장 큰 label 값

	//같은 그룹에 속한 픽셀은 같은 label값 갖도록 만들어주기
	for (k = 1; k <= num; k++) {
		if (k != r[k]) {
			r[k] = r[r[k]];
			Area[r[k]] += Area[k];
			Area[k] = 0;
		}
	}

	//물체 픽셀에 대해 label값을 r값으로 대체하기
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			if (Label[y][x] > 0) Label[y][x] = r[Label[y][x]];
		}
	}

	free(r); //r tabel 할당 해제하기

}

void Make_Noise_Removed_Image(BYTE **img1, int width, int height, int **Label, int *Area, int  Num, int m_LabelNum)
{
	int k, x, y;

	//noise 제거된 후의 영상이 img1이다.
	//noise 제거된 후의 영상 픽셀을 모두 0으로 초기화하기
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			img1[y][x] = 0;
		}
	}

	//m_LabelNum 이상의 픽셀개수인 그룹만 thresholding
	for (k = 1; k <= Num; k++) { //Num: 최대 label값
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

	//초기화
	Rc->top = 9999;
	Rc->bottom = 0;
	Rc->left = 9999;
	Rc->right = 0;

	//전체 label 영상 스캔
	//물체마다 x, y좌표의 최대, 최소값 찾기
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			if (Label[y][x] == k) { //label 값이 k일 때
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
	RECT Rc; //물체 정보를 담을 structure
	BYTE **img; //display할 영상

	yy0 = y0;
	for (k = 1; k <= Num; k++) {
		if (Area[k] >= m_Ref) { //픽셀이 모여져있는 개수가 m_Ref이상일 때 

			//Label영상에서 label값이 k인 것의 x,y좌표의 최대, 최소값을 Rc structure에(영역정보)저장.
			Find_Region(Label, width, height, k, &Rc);

			//display할 영상 정보
			w = Rc.right - Rc.left + 1; //가로길이
			h = Rc.bottom - Rc.top + 1; //세로길이
			img = cmatrix(h, w); //display 영상 할당하기

			//display영상 값 대입해주기
			for (y = 0; y < h; y++) {
				for (x = 0; x < w; x++) {
					if (Label[y + Rc.top][x + Rc.left] == k) img[y][x] = 255;
					else img[y][x] = 0;
				}
			}
			DisplayCimage2D(img, w, h, x0, yy0, TRUE); //물체영상 display하기
			yy0 += h + 10; //영상간의 간격은 10픽셀
			free_cmatrix(img, h, w); //영상 할당 해제하기
		}
	}

}

void CHomework::OnClickedLabeling()
{
	//각종 변수 선언하기
	int x0, y0, width, height, **Label, *Area, Num;
	BYTE **img1;

	if (!GetCurrentImageInfo(&width, &height, &x0, &y0)) return; //입력영상정보 읽기

	img1 = cmatrix(height, width); //입력영상 메모리 할당하기
	GetCurrentImageGray(img1); //입력영상정보 읽기

	Label = imatrix(height, width); //label영상 메모리 할당하기
	Area = (int *)malloc(width*height * sizeof(int)); //area값(그룹화된 픽셀 개수) 저장할 메모리 할당하기

	Image_Labeling(img1, width, height, Label, Area, &Num);//label영상 생성하기

	UpdateData(TRUE); //가장 최근에 업데이트된 값으로 가져온다.

	Make_Noise_Removed_Image(img1, width, height, Label, Area, Num, m_LabelNum);//label영상의 noise제거하기

	DisplayIimage2D(Label, width, height, x0, y0 + height, true); //label 영상 display하기
	DisplayCimage2D(img1, width, height, x0, y0 + height * 2, true); //noise 제거된 영상 display하기

	Display_Labeled_Images(Label, width, height, Area, Num, m_LabelNum, x0 + width + 10, y0); //라벨별로 물체 따로 display하기

	//메모리 할당해제
	free(Area); //area값(그룹화된 픽셀 개수) 저장할 메모리 할당 해제하기
	free_imatrix(Label, height, width);	//label영상 메모리 할당 해제하기
	free_cmatrix(img1, height, width); //noise제거영상 메모리 할당 해제하기
}
