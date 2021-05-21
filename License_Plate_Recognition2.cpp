void Character_Matching(BYTE **img, int w, int h, int *Num, float *Score)
{
	//w, h는 입력영상 정보

	//각종 변수 설정하기
	BYTE **Normal, **img2;
	int i, x, y, cnt;
	float max; //최대 matching score값 저장

	img2 = cmatrix(TmpH, TmpW); //template 영상 메모리 할당하기
	Normal = cmatrix(TmpH, TmpW); //Normalized 영상 메모리 할당하기

	Scale_Norm(img, Normal, w, h); //영상 크기 20x40으로 normalization

	max = 0;
	for (i = 0; i < 10; i++) {
		cnt = 0; //일치하는 픽셀 수 초기화
		for (y = 0; y < TmpH; y++) {
			for (x = 0; x < TmpW; x++) {
				if (CharTemplate[i][y][x] == 0) img2[y][x] = 0; //template 픽셀값이 0이면 0(검은색)
				else img2[y][x] = 255; //0이 아니면 255 (흰색)

				if (Normal[y][x] == img2[y][x]) cnt++; //일치하는 픽셀의 개수를 counting
			}
		}
		*Score = (float)cnt / 800; //marching score: 해당 숫자일 확률

		if (*Score > max) {// 가장 큰 matching score과 해당 숫자 찾기
			max = *Score;
			*Num = i;
		}
	}
	*Score = max; // 최종 matching score

	//메모리 할당 해제하기
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
	char msg[128]; //text를 display하기 위한 배열

	i = 0;
	xx0 = x0; //xx0: 인식된 숫자 display할 x좌표 위치
	yy0 = y0 + height + 5; //yy0: 인식된 숫자 display할 y좌표 위치
	max = 0;
	//RcNum = 0;

	for (k = 1; k <= Num; k++) { //Num: 최대 label 값
		if (Area[k] >= m_Ref) { //label별로 모여있는 화소의 개수가 100개이상이면
			Find_Region(Label, width, height, k, &Rc); //x,y좌표의 최대, 최소값 찾아서 Rc에 저장
			w = Rc.right - Rc.left + 1; //display할 영상의 가로 길이
			h = Rc.bottom - Rc.top + 1; //display할 영상의 세로 길이
			if (h < 50 && h > w) { //세로길이<50 이면서 세로길이>가로길이 이면
				img = cmatrix(h, w); //display할 영상 메모리 할당하기

				//Label값이 k인 작은 영상 만들기
				for (y = 0; y < h; y++) { //Label영상 스캔하기
					for (x = 0; x < w; x++) {
						if (Label[y + Rc.top][x + Rc.left] == k) img[y][x] = 255; //Label값이 k일때 물체지점이므로 밝게
						else img[y][x] = 0; //물체지점 아니면 어둡게
					}
				}

				Character_Matching(img, w, h, &Result, &Score); //입력영상 img의 최대 matching score계산
																//Result: 0~9 중 matching score 가장 큰 숫자

				if (Score >= 0.75) { //score가 0.75이상인 것만 display
					//Rlist[RcNum].left = Rc.left;
					//Rlist[RcNum].right = Rc.right;
					//Rlist[RcNum].top = Rc.top;
					//Rlist[RcNum++].bottom = Rc.bottom;

					sprintf(msg, "%d", Result); //Result: 인식된 숫자 
					yy = y1 + (int)((Rc.bottom + Rc.top)*0.5); //입력영상에 겹쳐지게 display할 y좌표 위치 
					dc->TextOut(x0 + Rc.left, yy, msg, strlen(msg)); //입력영상에 겹쳐지게 인식된 숫자 출력하기. 

					DisplayCimage2D(img, w, h, xx0, yy0, FALSE); //인식된 숫자영상 display하기
					sprintf(msg, "%d: %5.2f", Result, Score); //Result:  인식된 숫자, matching score: 인식확률
					dc->TextOut(xx0 + w + 5, yy0, msg, strlen(msg)); //숫자, matching score display하기

					//인식된 숫자 영상들을 정렬맞춰서 display하기
					i++;
					if (h > max) max = h; //max: 한 줄 내에 있는 영상들 중 가장 긴 세로길이 
					if (i >= 4) { //0번째부터 3번째까지, 즉 4개씩만 한 줄에 출력
						i = 0;
						yy0 += max + 10; //다음줄로 위치 넘기기
						xx0 = x0;
						max = 0;
					}
					else { //영상의 가로 간격 110 픽셀
						xx0 += 110;
					}
				}
				free_cmatrix(img, h, w); //메모리 할당 해제
			}
		}
	}

	pView->ReleaseDC(dc);
}

void CHomework::OnClickedLpr2()
{
	int x, y, x0, y0, width, height, **Label, *Area, Num;
	BYTE **Oimg, **Oimg1, **Oimg2; //Oimg: 입력영상, Oimg1: thresholding된 입력영상, Oimg2: reverse된 입력영상

	if (!GetCurrentImageInfo(&width, &height, &x0, &y0)) return; //입력영상정보 읽기

	Oimg = cmatrix(height, width); //입력영상 메모리 할당하기
	GetCurrentImageGray(Oimg); //입력영상정보 읽기

	Oimg1 = cmatrix(height, width); //출력영상 메모리 할당하기(reverse 전 영상)
	Oimg2 = cmatrix(height, width); //출력영상 메모리 할당하기(reverse 후 영상)

	//입력영상을 136 기준으로 thresholding(reverse 전) 
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			if (Oimg[y][x] >= 136) Oimg1[y][x] = 255; //픽셀값이 136 이상이면 흰색
			else Oimg1[y][x] = 0; //그 외에는 검정색
		}
	}

	//입력영상 reverse시키기
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			Oimg2[y][x] = 255 - Oimg[y][x];
		}
	}

	//입력영상을 136 기준으로 thresholding(reverse 후)
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			if (Oimg2[y][x] >= 136) Oimg2[y][x] = 255; //픽셀값이 136 이상이면 흰색
			else Oimg2[y][x] = 0; //그 외에는 검정색
		}
	}

	Label = imatrix(height, width); //Label 영상 할당하기
	Area = (int *)malloc(width*height * sizeof(int)); //area값(그룹화된 픽셀 개수) 저장할 메모리 할당하기

	//reverse 전
	Image_Labeling(Oimg1, width, height, Label, Area, &Num); //image labeling 진행하여 물체 형상 추출하기
	Make_Noise_Removed_Image(Oimg1, width, height, Label, Area, Num, 100); //label영상의 noise제거하기
	DisplayCimage2D(Oimg1, width, height, x0 + width + 10, y0, FALSE); //noise 제거된 영상 display하기
	Read_Template(); //templates 읽어오기
	Numeric_Recognition(Oimg1, Label, width, height, Area, Num, 100, x0, y0, y0); //Num: 최대 Label 수, Area: 화소가 모여있는 개수

	//reverse 후
	Image_Labeling(Oimg2, width, height, Label, Area, &Num); //image labeling 진행하여 물체 형상 추출하기
	Make_Noise_Removed_Image(Oimg2, width, height, Label, Area, Num, 100); //label영상의 noise제거하기
	DisplayCimage2D(Oimg2, width, height, x0 + width + 10, y0 + height + 10, FALSE); //noise 제거된 영상 display하기
	Read_Template(); //templates 읽어오기
	Numeric_Recognition(Oimg2, Label, width, height, Area, Num, 100, x0, y0 + 250, y0); //Num: 최대 Label 수, Area: 화소가 모여있는 개수

	//메모리 할당 해제하기																
	free(Area);
	free_imatrix(Label, height, width);
	free_cmatrix(Oimg, height, width);
	free_cmatrix(Oimg1, height, width);
	free_cmatrix(Oimg2, height, width);
}
