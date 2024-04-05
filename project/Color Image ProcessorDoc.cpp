
// Color Image ProcessorDoc.cpp: CColorImageProcessorDoc 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Color Image Processor.h"
#endif

#include "Color Image ProcessorDoc.h"
#include "SingleConstDlg.h"
#include "DualConstDlg.h"
#include "ComboConstDlg.h"
#include "ComboDlg.h"
#include "ColorConstDlg.h"

#include <propkey.h>
#include <cmath>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 사용자 선언

#define M_PI 3.14159265358979323846
#define RADIAN(x) ((x) * M_PI / 180.)
#define HUE(x) ((x) / (2 * M_PI) * 360)


// CColorImageProcessorDoc

IMPLEMENT_DYNCREATE(CColorImageProcessorDoc, CDocument)

BEGIN_MESSAGE_MAP(CColorImageProcessorDoc, CDocument)

END_MESSAGE_MAP()


// CColorImageProcessorDoc 생성/소멸

CColorImageProcessorDoc::CColorImageProcessorDoc() noexcept
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CColorImageProcessorDoc::~CColorImageProcessorDoc()
{
}


// CColorImageProcessorDoc 문서 작업

BOOL CColorImageProcessorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	DeleteInputImage();
	DeleteSubImage();
	DeleteOutputImage();

	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}

void CColorImageProcessorDoc::OnCloseDocument()
{
	DeleteInputImage();
	DeleteSubImage();
	DeleteOutputImage();

	CDocument::OnCloseDocument();
}

BOOL CColorImageProcessorDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// 기존 메모리 해제
	DeleteInputImage();
	DeleteSubImage();
	DeleteOutputImage();

	// 이미지 불러오기
	CImage image;
	image.Load(lpszPathName);

	// 이미지 크기
	inputHeight = image.GetHeight();
	inputWidth = image.GetWidth();

	// 메모리 할당
	inputImage = Create2DArray(inputHeight, inputWidth, RGB(0, 0, 0));

	for (int ypos = 0; ypos < inputHeight; ++ypos) {
		for (int xpos = 0; xpos < inputWidth; ++xpos) {
			inputImage[ypos][xpos] = image.GetPixel(xpos, ypos);
		}
	}

	return TRUE;
}

BOOL CColorImageProcessorDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// 출력 이미지 검사
	if (outputImage == NULL) {
		AfxMessageBox(_T("저장할 이미지가 없습니다."));
		return FALSE;
	}

	CFileDialog dialog(FALSE, _T("png"), NULL, OFN_HIDEREADONLY, _T("PNG 파일 (*.PNG)|*.PNG;*.png||"));
	if (dialog.DoModal() != IDOK) {
		return FALSE;
	}

	// 저장할 이미지 생성
	CImage image;
	image.Create(outputWidth, outputHeight, 32);

	for (int ypos = 0; ypos < outputHeight; ++ypos) {
		for (int xpos = 0; xpos < outputWidth; ++xpos) {
			image.SetPixel(xpos, ypos, outputImage[ypos][xpos]);
		}
	}

	// PNG 파일로 저장
	image.Save(dialog.GetPathName(), Gdiplus::ImageFormatPNG);
	CString str;
	str.Format(_T("%s 저장 성공"), dialog.GetPathName());
	MessageBox(NULL, str, _T("저장"), NULL);

	return TRUE;
	//return CDocument::OnSaveDocument(lpszPathName);
}


// CColorImageProcessorDoc serialization

void CColorImageProcessorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CColorImageProcessorDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CColorImageProcessorDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CColorImageProcessorDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CColorImageProcessorDoc 진단

#ifdef _DEBUG
void CColorImageProcessorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CColorImageProcessorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CColorImageProcessorDoc 명령

// 보조 이미지 열기
BOOL CColorImageProcessorDoc::OpenSubfile()
{
	if (inputImage == NULL) {
		AfxMessageBox(_T("주 이미지를 먼저 불러오세요."));
		return FALSE;
	}

	// 파일 선택
	CFileDialog dialog(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("이미지 파일(*.BMP, *.JPG, *.PNG, *.GIF) | *.BMP;*.JPG;*.PNG;*.GIF;*.bmp;*.jpg;*.png;*.gif |모든파일(*.*)|*.*||"));
	if (dialog.DoModal() != IDOK) {
		return FALSE;
	}

	// 기존 메모리 해제
	DeleteSubImage();

	// 이미지 불러오기
	CImage image;
	image.Load(dialog.GetPathName());

	// 이미지 크기
	subHeight = image.GetHeight();
	subWidth = image.GetWidth();

	// 메모리 할당
	subImage = Create2DArray(subHeight, subWidth, RGB(0, 0, 0));

	for (int ypos = 0; ypos < subHeight; ++ypos) {
		for (int xpos = 0; xpos < subWidth; ++xpos) {
			subImage[ypos][xpos] = image.GetPixel(xpos, ypos);
		}
	}

	return TRUE;
}


// 이차원 배열 동적 할당
template <typename T>
T** CColorImageProcessorDoc::Create2DArray(int height, int width, T initValue)
{
	T** tempArray;

	tempArray = new T * [height];
	for (int i = 0; i < height; ++i) {
		tempArray[i] = new T[width];
	}

	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			tempArray[i][j] = initValue;
		}
	}

	return tempArray;
}


// 이차원 배열 삭제
template <typename T>
void CColorImageProcessorDoc::Delete2DArray(T** target, int height)
{
	if (target == NULL) {
		return;
	}

	for (int i = 0; i < height; ++i) {
		delete[] target[i];
	}
	delete[] target;
}


// 입력 이미지 삭제
void CColorImageProcessorDoc::DeleteInputImage()
{
	if (inputImage != NULL) {
		Delete2DArray(inputImage, inputHeight);
		inputImage = NULL;
		inputHeight = inputWidth = 0;
	}
}


// 보조 이미지 삭제
void CColorImageProcessorDoc::DeleteSubImage()
{
	if (subImage != NULL) {
		Delete2DArray(subImage, subHeight);
		subImage = NULL;
		subHeight = subWidth = 0;
	}
}


// 출력 이미지 삭제
void CColorImageProcessorDoc::DeleteOutputImage()
{
	if (outputImage != NULL) {
		Delete2DArray(outputImage, outputHeight);
		outputImage = NULL;
		outputHeight = outputWidth = 0;
	}
}


// 입력 이미지를 그레이스케일로 변환한 이미지 생성
COLORREF** CColorImageProcessorDoc::CreateGrayscaleImage()
{
	COLORREF** tempArray = Create2DArray(inputHeight, inputWidth, RGB(0, 0, 0));

	// 처리
	double outPx;
	int color;
	for (int ypos = 0; ypos < inputHeight; ++ypos) {
		for (int xpos = 0; xpos < inputWidth; ++xpos) {
			outPx = (GetRValue(inputImage[ypos][xpos]) + GetGValue(inputImage[ypos][xpos]) + GetBValue(inputImage[ypos][xpos])) / 3.;
			outPx = Clamp(outPx, 0., 255.);
			color = (int)outPx;

			tempArray[ypos][xpos] = RGB(color, color, color);
		}
	}

	return tempArray;
}


// 마스크를 적용한 이미지 생성 (COLORREF, 3x3)
COLORREF** CColorImageProcessorDoc::CreateMaskedImage(double mask[][3], COLORREF** target, double addValue = 0)
{
	double** tempMask = Create2DArray(3, 3, 0.);
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			tempMask[i][j] = mask[i][j];
		}
	}

	COLORREF** tempOutputImage = CreateMaskedImage(tempMask, 3, target, addValue);

	Delete2DArray(tempMask, 3);

	return tempOutputImage;
}


// 마스크를 적용한 이미지 생성 (COLORREF, NxN)
COLORREF** CColorImageProcessorDoc::CreateMaskedImage(double** mask, int size, COLORREF** target, double addValue = 0)
{
	int maskSize = size;
	int maskExtend = maskSize / 2;

	// 임시 메모리 할당
	COLORREF** tempInputImage = Create2DArray(inputHeight + maskExtend * 2, inputWidth + maskExtend * 2, RGB(127, 127, 127));
	for (int ypos = 0; ypos < inputHeight; ++ypos) {
		for (int xpos = 0; xpos < inputWidth; ++xpos) {
			tempInputImage[ypos + maskExtend][xpos + maskExtend] = target[ypos][xpos];
		}
	}
	COLORREF** tempOutputImage = Create2DArray(outputHeight, outputWidth, RGB(0, 0, 0));

	// 회선 처리
	double outR, outG, outB;
	for (int ypos = 0; ypos < inputHeight; ++ypos) {
		for (int xpos = 0; xpos < inputWidth; ++xpos) {
			outR = outG = outB = 0;

			for (int i = 0; i < maskSize; ++i) {
				for (int j = 0; j < maskSize; ++j) {
					outR += GetRValue(tempInputImage[ypos + i][xpos + j]) * mask[i][j];
					outG += GetGValue(tempInputImage[ypos + i][xpos + j]) * mask[i][j];
					outB += GetBValue(tempInputImage[ypos + i][xpos + j]) * mask[i][j];
				}
			}

			outR = Clamp(outR + addValue, 0., 255.);
			outG = Clamp(outG + addValue, 0., 255.);
			outB = Clamp(outB + addValue, 0., 255.);
			tempOutputImage[ypos][xpos] = RGB((int)outR, (int)outG, (int)outB);
		}
	}

	// 임시 메모리 해제
	Delete2DArray(tempInputImage, inputHeight + maskExtend * 2);

	return tempOutputImage;
}


// 마스크를 적용한 이미지 생성 (double, 3x3)
double** CColorImageProcessorDoc::CreateMaskedImage(double mask[][3], double** target, double addValue = 0)
{
	int maskSize = 3;
	int maskExtend = maskSize / 2;
	
	// 임시 메모리 할당
	double** tempInputImage = Create2DArray(inputHeight + maskExtend * 2, inputWidth + maskExtend * 2, 127.);
	for (int ypos = 0; ypos < inputHeight; ++ypos) {
		for (int xpos = 0; xpos < inputWidth; ++xpos) {
			tempInputImage[ypos + maskExtend][xpos + maskExtend] = target[ypos][xpos];
		}
	}
	double** tempOutputImage = Create2DArray(outputHeight, outputWidth, 0.);

	// 회선 처리
	double outPx;
	for (int ypos = 0; ypos < inputHeight; ++ypos) {
		for (int xpos = 0; xpos < inputWidth; ++xpos) {
			outPx = 0;

			for (int i = 0; i < maskSize; ++i) {
				for (int j = 0; j < maskSize; ++j) {
					outPx += tempInputImage[ypos + i][xpos + j] * mask[i][j];
				}
			}

			outPx = Clamp(outPx + addValue, 0., 255.);
			tempOutputImage[ypos][xpos] = outPx;
		}
	}

	// 임시 메모리 해제
	Delete2DArray(tempInputImage, inputHeight + maskExtend * 2);

	return tempOutputImage;
}


// 값을 범위 내로 고정
template <typename T>
T CColorImageProcessorDoc::Clamp(T value, T min, T max)
{
	if (value > max)
		return max;
	else if (value < min)
		return min;
	else
		return value;
}


// RGB 색상을 HSI 색상으로 변환 (COLORREF)
double* CColorImageProcessorDoc::RGB2HSI(COLORREF rgb)
{
	double* hsi = new double[3];
	double h, s, i;

	double r = GetRValue(rgb);
	double g = GetGValue(rgb);
	double b = GetBValue(rgb);

	//  Code from:
	//  https://gist.github.com/KernelA/b462a49200c26028344783edaa639b2d
	
	i = (r + g + b) / 3.0;

	double rn = r / (r + g + b);
	double gn = g / (r + g + b);
	double bn = b / (r + g + b);

	h = acos((0.5 * ((rn - gn) + (rn - bn))) / (sqrt((rn - gn) * (rn - gn) + (rn - bn) * (gn - bn))));
	if (b > g)
	{
		h = 2 * M_PI - h;
	}

	s = 1 - 3 * min(rn, min(gn, bn));

	//////

	hsi[0] = h;
	hsi[1] = s;
	hsi[2] = i;

	return hsi;
}

// RGB 색상을 HSI 색상으로 변환 (int)
double* CColorImageProcessorDoc::RGB2HSI(int r, int g, int b)
{
	return RGB2HSI(RGB(r, g, b));
}


// HSI 색상을 RGB 색상으로 변환
COLORREF CColorImageProcessorDoc::HSI2RGB(double h, double s, double i)
{
	double r, g, b;

	//  Code from:
	//  https://gist.github.com/KernelA/b462a49200c26028344783edaa639b2d

	double x = i * (1 - s);

	// For black, white and grayscale h is NaN. Conversion works incorrectly.
	if (std::isnan(h))
	{
		r = i;
		g = i;
		b = i;
	}
	else if (h < 2 * M_PI / 3)
	{
		double y = i * (1 + (s * cos(h)) / (cos(M_PI / 3 - h)));
		double z = 3 * i - (x + y);
		b = x; r = y; g = z;
	}
	else if (h < 4 * M_PI / 3)
	{
		double y = i * (1 + (s * cos(h - 2 * M_PI / 3)) / (cos(M_PI / 3 - (h - 2 * M_PI / 3))));
		double z = 3 * i - (x + y);
		r = x; g = y; b = z;
	}
	else
	{
		double y = i * (1 + (s * cos(h - 4 * M_PI / 3)) / (cos(M_PI / 3 - (h - 4 * M_PI / 3))));
		double z = 3 * i - (x + y);
		r = z; g = x; b = y;
	}

	//////

	r = Clamp(r, 0., 255.);
	g = Clamp(g, 0., 255.);
	b = Clamp(b, 0., 255.);

	return RGB((int)r, (int)g, (int)b);
}


// 화소점 처리 - 동일
BOOL CColorImageProcessorDoc::ProcessPointEqual()
{
	// 입력 이미지 검사
	if (inputImage == NULL) {
		AfxMessageBox(_T("불러온 이미지가 없습니다."));
		return FALSE;
	}

	// 기존 출력 이미지 삭제
	DeleteOutputImage();

	// 메모리 할당
	outputHeight = inputHeight;
	outputWidth = inputWidth;
	outputImage = Create2DArray(outputHeight, outputWidth, RGB(0, 0, 0));

	// 처리
	for (int ypos = 0; ypos < inputHeight; ++ypos) {
		for (int xpos = 0; xpos < inputWidth; ++xpos) {
			outputImage[ypos][xpos] = inputImage[ypos][xpos];
		}
	}

	return TRUE;
}


// 화소점 처리 - 그레이스케일
BOOL CColorImageProcessorDoc::ProcessPointGrayscale()
{
	// 입력 이미지 검사
	if (inputImage == NULL) {
		AfxMessageBox(_T("불러온 이미지가 없습니다."));
		return FALSE;
	}

	// 기존 출력 이미지 삭제
	DeleteOutputImage();

	// 메모리 할당
	outputHeight = inputHeight;
	outputWidth = inputWidth;
	outputImage = Create2DArray(outputHeight, outputWidth, RGB(0, 0, 0));

	// 처리
	double outPx;
	int color;
	for (int ypos = 0; ypos < inputHeight; ++ypos) {
		for (int xpos = 0; xpos < inputWidth; ++xpos) {
			outPx = (GetRValue(inputImage[ypos][xpos]) + GetGValue(inputImage[ypos][xpos]) + GetBValue(inputImage[ypos][xpos])) / 3.;
			outPx = Clamp(outPx, 0., 255.);
			color = (int)outPx;

			outputImage[ypos][xpos] = RGB(color, color, color);
		}
	}

	return TRUE;
}


// 화소점 처리 - 밝기
BOOL CColorImageProcessorDoc::ProcessPointBrightness()
{
	// 입력 이미지 검사
	if (inputImage == NULL) {
		AfxMessageBox(_T("불러온 이미지가 없습니다."));
		return FALSE;
	}

	// 입력값 (-255~255)
	CSingleConstDlg dialog((CWnd*)nullptr, _T("밝기"), _T("-255~255 사이의 정수 입력"), -255, 255);
	if (dialog.DoModal() != IDOK) {
		return FALSE;
	}
	int value = (int)dialog.value;

	// 기존 출력 이미지 삭제
	DeleteOutputImage();

	// 메모리 할당
	outputHeight = inputHeight;
	outputWidth = inputWidth;
	outputImage = Create2DArray(outputHeight, outputWidth, RGB(0, 0, 0));

	double** tempInputImageH = Create2DArray(inputHeight, inputWidth, 0.);
	double** tempInputImageS = Create2DArray(inputHeight, inputWidth, 0.);
	double** tempInputImageI = Create2DArray(inputHeight, inputWidth, 0.);
	double** tempOutputImageI = Create2DArray(outputHeight, outputWidth, 0.);

	// RGB -> HSI 변환
	for (int ypos = 0; ypos < inputHeight; ++ypos) {
		for (int xpos = 0; xpos < inputWidth; ++xpos) {
			double* hsi;
			hsi = RGB2HSI(inputImage[ypos][xpos]);

			tempInputImageH[ypos][xpos] = hsi[0];
			tempInputImageS[ypos][xpos] = hsi[1];
			tempInputImageI[ypos][xpos] = hsi[2];
			delete[] hsi;
		}
	}

	// 처리
	int outPx;
	for (int ypos = 0; ypos < inputHeight; ++ypos) {
		for (int xpos = 0; xpos < inputWidth; ++xpos) {
			outPx = (int)tempInputImageI[ypos][xpos] + value;
			outPx = Clamp(outPx, 0, 255);

			tempOutputImageI[ypos][xpos] = outPx;
		}
	}

	// HSI -> RGB 변환
	for (int ypos = 0; ypos < inputHeight; ++ypos) {
		for (int xpos = 0; xpos < inputWidth; ++xpos) {
			double h, s, i;
			h = tempInputImageH[ypos][xpos];
			s = tempInputImageS[ypos][xpos];
			i = tempOutputImageI[ypos][xpos];

			outputImage[ypos][xpos] = HSI2RGB(h, s, i);
		}
	}

	// 임시 메모리 해제
	Delete2DArray(tempInputImageH, inputHeight);
	Delete2DArray(tempInputImageS, inputHeight);
	Delete2DArray(tempInputImageI, inputHeight);
	Delete2DArray(tempOutputImageI, outputHeight);

	return TRUE;
}


// 화소점 처리 - 반전
BOOL CColorImageProcessorDoc::ProcessPointInvert()
{
	// 입력 이미지 검사
	if (inputImage == NULL) {
		AfxMessageBox(_T("불러온 이미지가 없습니다."));
		return FALSE;
	}

	// 기존 출력 이미지 삭제
	DeleteOutputImage();

	// 메모리 할당
	outputHeight = inputHeight;
	outputWidth = inputWidth;
	outputImage = Create2DArray(outputHeight, outputWidth, RGB(0, 0, 0));

	// 처리
	for (int ypos = 0; ypos < inputHeight; ++ypos) {
		for (int xpos = 0; xpos < inputWidth; ++xpos) {
			COLORREF outPx = RGB(255, 255, 255) - inputImage[ypos][xpos];
			outputImage[ypos][xpos] = outPx;
		}
	}

	return TRUE;
}


// 화소점 처리 - 흑백
BOOL CColorImageProcessorDoc::ProcessPointThreshold()
{
	// 입력 이미지 검사
	if (inputImage == NULL) {
		AfxMessageBox(_T("불러온 이미지가 없습니다."));
		return FALSE;
	}

	// 입력값
	CSingleConstDlg dialog((CWnd*)nullptr, _T("흑백"), _T("0~255 사이의 정수 입력"), 0, 255);
	if (dialog.DoModal() != IDOK) {
		return FALSE;
	}
	int value = (int)dialog.value;

	// 기존 출력 이미지 삭제
	DeleteOutputImage();

	// 메모리 할당
	outputHeight = inputHeight;
	outputWidth = inputWidth;
	outputImage = Create2DArray(outputHeight, outputWidth, RGB(0, 0, 0));

	// 처리
	double outPx;
	int color;
	for (int ypos = 0; ypos < inputHeight; ++ypos) {
		for (int xpos = 0; xpos < inputWidth; ++xpos) {
			outPx = (GetRValue(inputImage[ypos][xpos]) + GetGValue(inputImage[ypos][xpos]) + GetBValue(inputImage[ypos][xpos])) / 3.;
			outPx = Clamp(outPx, 0., 255.);
			color = (int)outPx;
			
			if (color < value)
				color = 0;
			else
				color = 255;

			outputImage[ypos][xpos] = RGB(color, color, color);
		}
	}

	return TRUE;
}


// 화소점 처리 - 감마
BOOL CColorImageProcessorDoc::ProcessPointGamma()
{
	// 입력 이미지 검사
	if (inputImage == NULL) {
		AfxMessageBox(_T("불러온 이미지가 없습니다."));
		return FALSE;
	}

	// 입력값 (0~)
	CSingleConstDlg dialog((CWnd*)nullptr, _T("감마"), _T("0 이상의 실수 입력"), 0, 9999999);
	if (dialog.DoModal() != IDOK) {
		return FALSE;
	}
	double value = dialog.value;

	// 기존 출력 이미지 삭제
	DeleteOutputImage();

	// 메모리 할당
	outputHeight = inputHeight;
	outputWidth = inputWidth;
	outputImage = Create2DArray(outputHeight, outputWidth, RGB(0, 0, 0));

	double** tempInputImageH = Create2DArray(inputHeight, inputWidth, 0.);
	double** tempInputImageS = Create2DArray(inputHeight, inputWidth, 0.);
	double** tempInputImageI = Create2DArray(inputHeight, inputWidth, 0.);
	double** tempOutputImageI = Create2DArray(outputHeight, outputWidth, 0.);

	// RGB -> HSI 변환
	for (int ypos = 0; ypos < inputHeight; ++ypos) {
		for (int xpos = 0; xpos < inputWidth; ++xpos) {
			double* hsi;
			hsi = RGB2HSI(inputImage[ypos][xpos]);

			tempInputImageH[ypos][xpos] = hsi[0];
			tempInputImageS[ypos][xpos] = hsi[1];
			tempInputImageI[ypos][xpos] = hsi[2];
			delete[] hsi;
		}
	}

	// 처리
	double outPx;
	for (int ypos = 0; ypos < inputHeight; ++ypos) {
		for (int xpos = 0; xpos < inputWidth; ++xpos) {
			if (value == 0) {
				tempOutputImageI[ypos][xpos] = 255;
			}
			else {
				outPx = pow(tempOutputImageI[ypos][xpos], 1 / value);
				outPx = Clamp(outPx, 0., 255.);

				tempOutputImageI[ypos][xpos] = outPx;
			}
		}
	}

	// HSI -> RGB 변환
	for (int ypos = 0; ypos < inputHeight; ++ypos) {
		for (int xpos = 0; xpos < inputWidth; ++xpos) {
			double h, s, i;
			h = tempInputImageH[ypos][xpos];
			s = tempInputImageS[ypos][xpos];
			i = tempOutputImageI[ypos][xpos];

			outputImage[ypos][xpos] = HSI2RGB(h, s, i);
		}
	}

	// 임시 메모리 해제
	Delete2DArray(tempInputImageH, inputHeight);
	Delete2DArray(tempInputImageS, inputHeight);
	Delete2DArray(tempInputImageI, inputHeight);
	Delete2DArray(tempOutputImageI, outputHeight);

	// 처리
	double outR, outG, outB;
	for (int ypos = 0; ypos < inputHeight; ++ypos) {
		for (int xpos = 0; xpos < inputWidth; ++xpos) {
			if (value == 0) {
				outputImage[ypos][xpos] = RGB(255, 255, 255);
			}
			else {
				outR = pow(GetRValue(inputImage[ypos][xpos]), 1 / value);
				outG = pow(GetGValue(inputImage[ypos][xpos]), 1 / value);
				outB = pow(GetBValue(inputImage[ypos][xpos]), 1 / value);

				outR = Clamp(outR, 0., 255.);
				outG = Clamp(outG, 0., 255.);
				outB = Clamp(outB, 0., 255.);

				outputImage[ypos][xpos] = RGB((int)outR, (int)outG, (int)outB);
			}
		}
	}

	return TRUE;
}


// 화소점 처리 - 대비
BOOL CColorImageProcessorDoc::ProcessPointContrast()
{
	// 입력 이미지 검사
	if (inputImage == NULL) {
		AfxMessageBox(_T("불러온 이미지가 없습니다."));
		return FALSE;
	}

	// 입력값 (-100~100)
	CSingleConstDlg dialog((CWnd*)nullptr, _T("대비"), _T("-100~100 사이의 정수 입력"), -100, 100);
	if (dialog.DoModal() != IDOK) {
		return FALSE;
	}
	int value = (int)dialog.value;

	// 기존 출력 이미지 삭제
	DeleteOutputImage();

	// 메모리 할당
	outputHeight = inputHeight;
	outputWidth = inputWidth;
	outputImage = Create2DArray(outputHeight, outputWidth, RGB(0, 0, 0));

	// 수식
	double a, b;
	if (value > 0) {
		double delta = 127. * value / 100;
		a = 255 / (255 - delta * 2);
		b = -delta * a;
	}
	else {
		double delta = -128. * value / 100;
		a = (256 - delta * 2) / 255;
		b = delta;
	}

	// 처리
	double inR, inG, inB;
	double outR, outG, outB;
	for (int ypos = 0; ypos < inputHeight; ++ypos) {
		for (int xpos = 0; xpos < inputWidth; ++xpos) {
			inR = (double)GetRValue(inputImage[ypos][xpos]);
			inG = (double)GetGValue(inputImage[ypos][xpos]);
			inB = (double)GetBValue(inputImage[ypos][xpos]);

			outR = Clamp(a * inR + b, 0., 255.);
			outG = Clamp(a * inG + b, 0., 255.);
			outB = Clamp(a * inB + b, 0., 255.);

			outputImage[ypos][xpos] = RGB((int)outR, (int)outG, (int)outB);
		}
	}

	return TRUE;
}


// 화소점 처리 - 포스터라이즈
BOOL CColorImageProcessorDoc::ProcessPointPosterize()
{
	// 입력 이미지 검사
	if (inputImage == NULL) {
		AfxMessageBox(_T("불러온 이미지가 없습니다."));
		return FALSE;
	}

	// 입력값 (2~256)
	CSingleConstDlg dialog((CWnd*)nullptr, _T("포스터라이즈"), _T("2~256 사이의 정수 입력"), 2, 256);
	if (dialog.DoModal() != IDOK) {
		return FALSE;
	}
	int level = (int)dialog.value;
	int border = 256 / level;
	int interval = 256 / (level - 1);

	// 기존 출력 이미지 삭제
	DeleteOutputImage();

	// 메모리 할당
	outputHeight = inputHeight;
	outputWidth = inputWidth;
	outputImage = Create2DArray(outputHeight, outputWidth, RGB(0, 0, 0));

	double** tempInputImageH = Create2DArray(inputHeight, inputWidth, 0.);
	double** tempInputImageS = Create2DArray(inputHeight, inputWidth, 0.);
	double** tempInputImageI = Create2DArray(inputHeight, inputWidth, 0.);
	double** tempOutputImageI = Create2DArray(outputHeight, outputWidth, 0.);

	// RGB -> HSI 변환
	for (int ypos = 0; ypos < inputHeight; ++ypos) {
		for (int xpos = 0; xpos < inputWidth; ++xpos) {
			double* hsi;
			hsi = RGB2HSI(inputImage[ypos][xpos]);

			tempInputImageH[ypos][xpos] = hsi[0];
			tempInputImageS[ypos][xpos] = hsi[1];
			tempInputImageI[ypos][xpos] = hsi[2];
			delete[] hsi;
		}
	}

	// 처리
	int outPx;
	for (int ypos = 0; ypos < inputHeight; ++ypos) {
		for (int xpos = 0; xpos < inputWidth; ++xpos) {
			outPx = ((int)tempInputImageI[ypos][xpos] / border) * interval;
			outPx = Clamp(outPx, 0, 255);

			tempOutputImageI[ypos][xpos] = outPx;
		}
	}

	// HSI -> RGB 변환
	for (int ypos = 0; ypos < inputHeight; ++ypos) {
		for (int xpos = 0; xpos < inputWidth; ++xpos) {
			double h, s, i;
			h = tempInputImageH[ypos][xpos];
			s = tempInputImageS[ypos][xpos];
			i = tempOutputImageI[ypos][xpos];

			outputImage[ypos][xpos] = HSI2RGB(h, s, i);
		}
	}

	// 임시 메모리 해제
	Delete2DArray(tempInputImageH, inputHeight);
	Delete2DArray(tempInputImageS, inputHeight);
	Delete2DArray(tempInputImageI, inputHeight);
	Delete2DArray(tempOutputImageI, outputHeight);

	return TRUE;
}


// 화소점 처리 - 산술 연산
BOOL CColorImageProcessorDoc::ProcessPointMath(int opType)
{
	// 입력 이미지 검사
	if (inputImage == NULL) {
		AfxMessageBox(_T("불러온 이미지가 없습니다."));
		return FALSE;
	}

	// 보조 이미지 검사
	if (subImage == NULL) {
		AfxMessageBox(_T("보조 이미지가 없습니다."));
		return FALSE;
	}

	// 기존 출력 이미지 삭제
	DeleteOutputImage();

	// 메모리 할당
	outputHeight = inputHeight;
	outputWidth = inputWidth;
	outputImage = Create2DArray(outputHeight, outputWidth, RGB(0, 0, 0));

	// 처리
	int inR, inG, inB;
	int subR, subG, subB;
	int outR, outG, outB;
	for (int ypos = 0; ypos < outputHeight; ++ypos) {
		for (int xpos = 0; xpos < outputWidth; ++xpos) {
			if (ypos < subHeight && xpos < subWidth) {
				inR = GetRValue(inputImage[ypos][xpos]);
				inG = GetGValue(inputImage[ypos][xpos]);
				inB = GetBValue(inputImage[ypos][xpos]);
				subR = GetRValue(subImage[ypos][xpos]);
				subG = GetGValue(subImage[ypos][xpos]);
				subB = GetBValue(subImage[ypos][xpos]);

				switch (opType)
				{
				case 0:			// 더하기
					outR = Clamp(inR + subR, 0, 255);
					outG = Clamp(inG + subG, 0, 255);
					outB = Clamp(inB + subB, 0, 255);
					break;
				case 1:			// 빼기
					outR = Clamp(inR - subR, 0, 255);
					outG = Clamp(inG - subG, 0, 255);
					outB = Clamp(inB - subB, 0, 255);
					break;
				case 2:			// 곱하기
					outR = (int)Clamp(inR * (subR / 255.), 0., 255.);
					outG = (int)Clamp(inG * (subG / 255.), 0., 255.);
					outB = (int)Clamp(inB * (subB / 255.), 0., 255.);
					break;
				case 3:			// 나누기
					outR = subR == 0 ? 255 : (int)Clamp(inR / (subR / 255.), 0., 255.);
					outG = subG == 0 ? 255 : (int)Clamp(inG / (subG / 255.), 0., 255.);
					outB = subB == 0 ? 255 : (int)Clamp(inB / (subB / 255.), 0., 255.);
					break;
				default:
					AfxMessageBox(_T("잘못된 파라미터 값"));
					return FALSE;
				}

				outputImage[ypos][xpos] = RGB(outR, outG, outB);
			}
			else {
				outputImage[ypos][xpos] = inputImage[ypos][xpos];
			}
		}
	}

	return TRUE;
}


// 화소점 처리 - 논리 연산
BOOL CColorImageProcessorDoc::ProcessPointLogic(int logicType)
{
	// 입력 이미지 검사
	if (inputImage == NULL) {
		AfxMessageBox(_T("불러온 이미지가 없습니다."));
		return FALSE;
	}

	// 보조 이미지 검사
	if (subImage == NULL) {
		AfxMessageBox(_T("보조 이미지가 없습니다."));
		return FALSE;
	}

	// 기존 출력 이미지 삭제
	DeleteOutputImage();

	// 메모리 할당
	outputHeight = inputHeight;
	outputWidth = inputWidth;
	outputImage = Create2DArray(outputHeight, outputWidth, RGB(0, 0, 0));

	int h = min(subHeight, inputHeight);
	int w = min(subWidth, inputWidth);
	for (int ypos = 0; ypos < inputHeight; ++ypos) {
		for (int xpos = 0; xpos < inputWidth; ++xpos) {
			if (ypos < subHeight && xpos < subWidth) {
				switch (logicType)
				{
				case 0:		// AND
					outputImage[ypos][xpos] = inputImage[ypos][xpos] & subImage[ypos][xpos];
					break;
				case 1:		// OR
					outputImage[ypos][xpos] = inputImage[ypos][xpos] | subImage[ypos][xpos];
					break;
				case 2:		// XOR
					outputImage[ypos][xpos] = inputImage[ypos][xpos] ^ subImage[ypos][xpos];
					break;
				default:
					AfxMessageBox(_T("잘못된 파라미터 값"));
					return FALSE;
				}
			}
			else {
				outputImage[ypos][xpos] = inputImage[ypos][xpos];
			}
		}
	}

	return TRUE;
}


// 기하학 처리 - 확대
BOOL CColorImageProcessorDoc::ProcessGeometryZoomin()
{
	// 입력 이미지 검사
	if (inputImage == NULL) {
		AfxMessageBox(_T("불러온 이미지가 없습니다."));
		return FALSE;
	}

	// 입력값
	CStringArray items;
	items.Add(_T("최근접 이웃"));
	items.Add(_T("양선형 보간"));

	CComboConstDlg dialog((CWnd*)nullptr, _T("확대"), _T("보간 방법"), _T("배율 (1이상 정수)"), 1, 9999999, items);
	if (dialog.DoModal() != IDOK) {
		return FALSE;
	}
	int selection = dialog.selection;
	int scale = (int)dialog.value;

	// 기존 출력 이미지 삭제
	DeleteOutputImage();

	// 메모리 할당
	outputHeight = inputHeight * scale;
	outputWidth = inputWidth * scale;
	outputImage = Create2DArray(outputHeight, outputWidth, RGB(0, 0, 0));

	// 처리
	switch (selection)
	{
	case 0:			// 최근접 이웃
		GeometryZoominNeareast(scale);
		break;
	case 1:			// 평균값
		GeometryZoominBilinear(scale);
		break;
	default:
		AfxMessageBox(_T("잘못된 파라미터 값"));
		return FALSE;
	}

	return TRUE;
}


// 확대 - 최근접 이웃
void CColorImageProcessorDoc::GeometryZoominNeareast(int scale)
{
	int inY, inX;
	for (int ypos = 0; ypos < outputHeight; ++ypos) {
		for (int xpos = 0; xpos < outputWidth; ++xpos) {
			inY = min(ypos / scale, inputHeight - 1);
			inX = min(xpos / scale, inputWidth - 1);
			outputImage[ypos][xpos] = inputImage[inY][inX];
		}
	}
}


// 확대 - 양선형 보간
void CColorImageProcessorDoc::GeometryZoominBilinear(int scale)
{
	double ry, rx, sy, sx;
	int iy, ix;
	COLORREF c1, c2, c3, c4;
	double in1, in2, in3, in4;
	double outR, outG, outB;

	// TODO: 수정 필요
	for (int ypos = 0; ypos < outputHeight; ++ypos) {
		for (int xpos = 0; xpos < outputWidth; ++xpos) {
			ry = ypos / (double)scale;
			rx = xpos / (double)scale;
			iy = min((int)floor(ry), inputHeight - 2);
			ix = min((int)floor(rx), inputWidth - 2);
			sy = ry - iy;
			sx = rx - ix;

			if (0 <= iy && iy < (inputHeight - 1) && 0 <= ix && ix < (inputWidth - 1)) {
				c1 = inputImage[iy][ix];
				c2 = inputImage[iy][ix + 1];
				c3 = inputImage[iy + 1][ix + 1];
				c4 = inputImage[iy + 1][ix];

				// Red
				in1 = (double)GetRValue(c1);
				in2 = (double)GetRValue(c2);
				in3 = (double)GetRValue(c3);
				in4 = (double)GetRValue(c4);
				outR = in1 * (1 - sx) * (1 - sy) + in2 * sx * (1 - sy) + in3 * sx * sy + in4 * (1 - sx) * sy;
				outR = Clamp(outR, 0., 255.);

				// Green
				in1 = (double)GetGValue(c1);
				in2 = (double)GetGValue(c2);
				in3 = (double)GetGValue(c3);
				in4 = (double)GetGValue(c4);
				outG = in1 * (1 - sx) * (1 - sy) + in2 * sx * (1 - sy) + in3 * sx * sy + in4 * (1 - sx) * sy;
				outG = Clamp(outG, 0., 255.);

				// Blue
				in1 = (double)GetBValue(c1);
				in2 = (double)GetBValue(c2);
				in3 = (double)GetBValue(c3);
				in4 = (double)GetBValue(c4);
				outB = in1 * (1 - sx) * (1 - sy) + in2 * sx * (1 - sy) + in3 * sx * sy + in4 * (1 - sx) * sy;
				outB = Clamp(outB, 0., 255.);

				outputImage[ypos][xpos] = RGB((int)outR, (int)outG, (int)outB);
			}
			else {
				outputImage[ypos][xpos] = RGB(255, 255, 255);
			}
		}
	}
}


// 기하학 처리 - 축소
BOOL CColorImageProcessorDoc::ProcessGeometryZoomout()
{
	// 입력 이미지 검사
	if (inputImage == NULL) {
		AfxMessageBox(_T("불러온 이미지가 없습니다."));
		return FALSE;
	}

	// 입력값
	CStringArray items;
	items.Add(_T("최근접 이웃"));
	items.Add(_T("평균값"));

	CComboConstDlg dialog((CWnd*)nullptr, _T("축소"), _T("보간 방법"), _T("배율 (1이상 정수)"), 1, 9999999, items);
	if (dialog.DoModal() != IDOK) {
		return FALSE;
	}
	int selection = dialog.selection;
	int scale = (int)dialog.value;
	scale = min(scale, inputWidth);

	// 기존 출력 이미지 삭제
	DeleteOutputImage();

	// 메모리 할당
	outputHeight = inputHeight / scale;
	outputWidth = inputWidth / scale;
	outputImage = Create2DArray(outputHeight, outputWidth, RGB(0, 0, 0));

	// 처리
	switch (selection)
	{
	case 0:			// 최근접 이웃
		GeometryZoomoutNeareast(scale);
		break;
	case 1:			// 평균값
		GeometryZoomoutAvg(scale);
		break;
	default:
		AfxMessageBox(_T("잘못된 파라미터 값"));
		return FALSE;
	}

	return TRUE;
}


// 축소 - 최근접 이웃
void CColorImageProcessorDoc::GeometryZoomoutNeareast(int scale)
{
	int inY, inX;
	for (int ypos = 0; ypos < outputHeight; ++ypos) {
		for (int xpos = 0; xpos < outputWidth; ++xpos) {
			inY = min(ypos * scale, inputHeight - 1);
			inX = min(xpos * scale, inputWidth - 1);
			outputImage[ypos][xpos] = inputImage[inY][inX];
		}
	}
}


// 축소 - 평균값
void CColorImageProcessorDoc::GeometryZoomoutAvg(int scale)
{
	int sumR, sumG, sumB;
	int inY, inX;
	for (int ypos = 0; ypos < outputHeight; ++ypos) {
		for (int xpos = 0; xpos < outputWidth; ++xpos) {
			sumR = 0;
			sumG = 0;
			sumB = 0;

			for (int i = 0; i < scale; ++i) {
				for (int j = 0; j < scale; ++j) {
					inY = min(ypos * scale + i, inputHeight - 1);
					inX = min(xpos * scale + j, inputWidth - 1);

					sumR += GetRValue(inputImage[inY][inX]);
					sumG += GetGValue(inputImage[inY][inX]);
					sumB += GetBValue(inputImage[inY][inX]);
				}
			}

			sumR = Clamp(sumR / (scale * scale), 0, 255);
			sumG = Clamp(sumG / (scale * scale), 0, 255);
			sumB = Clamp(sumB / (scale * scale), 0, 255);

			outputImage[ypos][xpos] = RGB(sumR, sumG, sumB);
		}
	}
}


// 기하학 처리 - 회전
BOOL CColorImageProcessorDoc::ProcessGeometryRotate()
{
	// 입력 이미지 검사
	if (inputImage == NULL) {
		AfxMessageBox(_T("불러온 이미지가 없습니다."));
		return FALSE;
	}

	CStringArray items;
	items.Add(_T("원본 크기 유지"));
	items.Add(_T("확장"));

	CComboConstDlg dialog((CWnd*)nullptr, _T("회전"), _T("캔버스 크기"), _T("회전 각도"), -360, 360, items);
	if (dialog.DoModal() != IDOK) {
		return FALSE;
	}
	int angle = (int)dialog.value;
	bool isExpand = (bool)dialog.selection;

	// 기존 출력 이미지 삭제
	DeleteOutputImage();

	if (isExpand) {
		GeometryRotateExpand(angle);
	}
	else {
		GeometryRotateMaintain(angle);
	}

	return TRUE;
}


// 회전 - 캔버스 크기 유지
void CColorImageProcessorDoc::GeometryRotateMaintain(int angle)
{
	// 메모리 할당
	outputHeight = inputHeight;
	outputWidth = inputWidth;
	outputImage = Create2DArray(outputHeight, outputWidth, RGB(0, 0, 0));

	double rad = RADIAN(angle);
	int cy = inputHeight / 2;
	int cx = inputWidth / 2;

	// 처리
	double yd, xd, ys, xs;
	for (int ypos = 0; ypos < outputHeight; ++ypos) {
		for (int xpos = 0; xpos < outputWidth; ++xpos) {
			yd = ypos;
			xd = xpos;
			ys = (cos(rad) * (yd - cy) + sin(rad) * (xd - cx)) + cy;
			xs = (-sin(rad) * (yd - cy) + cos(rad) * (xd - cx)) + cx;

			if (0 <= ys && ys < outputHeight && 0 <= xs && xs < outputWidth) {
				outputImage[ypos][xpos] = inputImage[(int)ys][(int)xs];
			}
			else {
				outputImage[ypos][xpos] = RGB(0, 0, 0);
			}
		}
	}
}


// 회전 - 캔버스 크기 확장
void CColorImageProcessorDoc::GeometryRotateExpand(int angle)
{
	double r1 = RADIAN(angle);
	double r2 = RADIAN(90 - angle);

	// 출력 이미지 배열 생성
	outputHeight = (int)(inputHeight * abs(cos(r1)) + inputWidth * abs(cos(r2)));
	outputWidth = (int)(inputHeight * abs(cos(r2)) + inputWidth * abs(cos(r1)));
	outputImage = Create2DArray(outputHeight, outputWidth, RGB(0, 0, 0));

	double rad = angle * 3.141592 / 180.;
	int cy = outputHeight / 2;
	int cx = outputWidth / 2;
	int cy2 = inputHeight / 2;
	int cx2 = inputWidth / 2;

	// 처리
	double yd, xd, ys, xs;
	for (int ypos = 0; ypos < outputHeight; ++ypos) {
		for (int xpos = 0; xpos < outputWidth; ++xpos) {
			yd = ypos;
			xd = xpos;
			ys = (cos(rad) * (yd - cy) + sin(rad) * (xd - cx)) + cy2;
			xs = (-sin(rad) * (yd - cy) + cos(rad) * (xd - cx)) + cx2;

			if (0 <= ys && ys < inputHeight && 0 <= xs && xs < inputWidth) {
				outputImage[ypos][xpos] = inputImage[(int)ys][(int)xs];
			}
			else {
				outputImage[ypos][xpos] = RGB(0, 0, 0);
			}
		}
	}
}

// 기하학 처리 - 이동
BOOL CColorImageProcessorDoc::ProcessGeometryTranslate()
{
	// 입력 이미지 검사
	if (inputImage == NULL) {
		AfxMessageBox(_T("불러온 이미지가 없습니다."));
		return FALSE;
	}

	// 입력값
	CDualConstDlg dialog((CWnd*)nullptr, _T("이동"), _T("정수값 입력"), _T("X좌표"), _T("Y좌표"), -9999999, 9999999);
	if (dialog.DoModal() != IDOK) {
		return FALSE;
	}
	int moveX = (int)dialog.value1;
	int moveY = (int)dialog.value2;

	// 기존 출력 이미지 삭제
	DeleteOutputImage();

	// 메모리 할당
	outputHeight = inputHeight;
	outputWidth = inputWidth;
	outputImage = Create2DArray(outputHeight, outputWidth, RGB(0, 0, 0));

	// 처리
	for (int ypos = 0; ypos < outputHeight; ++ypos) {
		for (int xpos = 0; xpos < outputWidth; ++xpos) {
			int inY = ypos - moveY;
			int inX = xpos - moveX;

			if (0 <= inY && inY < inputHeight && 0 <= inX && inX < inputWidth) {
				outputImage[ypos][xpos] = inputImage[inY][inX];
			}
		}
	}

	return TRUE;
}


// 기하학 처리 - 대칭
BOOL CColorImageProcessorDoc::ProcessGeometryMirror(bool isVert)
{
	// 입력 이미지 검사
	if (inputImage == NULL) {
		AfxMessageBox(_T("불러온 이미지가 없습니다."));
		return FALSE;
	}

	// 기존 출력 이미지 삭제
	DeleteOutputImage();

	// 메모리 할당
	outputHeight = inputHeight;
	outputWidth = inputWidth;
	outputImage = Create2DArray(outputHeight, outputWidth, RGB(0, 0, 0));

	for (int ypos = 0; ypos < inputHeight; ++ypos) {
		for (int xpos = 0; xpos < inputWidth; ++xpos) {
			if (isVert) {
				int outY = inputHeight - 1 - ypos;
				outputImage[outY][xpos] = inputImage[ypos][xpos];
			}
			else {
				int outX = inputWidth - 1 - xpos;
				outputImage[ypos][outX] = inputImage[ypos][xpos];
			}
		}
	}

	return TRUE;
}


// 기하학 처리 - 모핑
BOOL CColorImageProcessorDoc::ProcessGeometryBlend()
{
	// 입력 이미지 검사
	if (inputImage == NULL) {
		AfxMessageBox(_T("불러온 이미지가 없습니다."));
		return FALSE;
	}

	// 보조 이미지 검사
	if (subImage == NULL) {
		AfxMessageBox(_T("보조 이미지가 없습니다."));
		return FALSE;
	}

	// 입력값 (0~1)
	CSingleConstDlg dialog((CWnd*)nullptr, _T("모핑"), _T("0~1 사이 실수 (1이 보조)"), 0, 1);
	if (dialog.DoModal() != IDOK) {
		return FALSE;
	}
	double value = dialog.value;

	// 기존 출력 이미지 삭제
	DeleteOutputImage();

	// 메모리 할당
	outputHeight = inputHeight;
	outputWidth = inputWidth;
	outputImage = Create2DArray(outputHeight, outputWidth, RGB(0, 0, 0));

	// 처리
	double inR, inG, inB;
	double subR, subG, subB;
	double outR, outG, outB;
	for (int ypos = 0; ypos < outputHeight; ++ypos) {
		for (int xpos = 0; xpos < outputWidth; ++xpos) {
			if (ypos < subHeight && xpos < subWidth) {
				inR = GetRValue(inputImage[ypos][xpos]) * (1 - value);
				inG = GetGValue(inputImage[ypos][xpos]) * (1 - value);
				inB = GetBValue(inputImage[ypos][xpos]) * (1 - value);
				subR = GetRValue(subImage[ypos][xpos]) * value;
				subG = GetGValue(subImage[ypos][xpos]) * value;
				subB = GetBValue(subImage[ypos][xpos]) * value;

				outR = Clamp(inR + subR, 0., 255.);
				outG = Clamp(inG + subG, 0., 255.);
				outB = Clamp(inB + subB, 0., 255.);

				outputImage[ypos][xpos] = RGB((int)outR, (int)outG, (int)outB);
			}
			else {
				outputImage[ypos][xpos] = inputImage[ypos][xpos];
			}
		}
	}

	return TRUE;
}


// 히스토그램 처리 - 히스토그램 스트레칭
BOOL CColorImageProcessorDoc::ProcessHistStretch()
{
	// 입력 이미지 검사
	if (inputImage == NULL) {
		AfxMessageBox(_T("불러온 이미지가 없습니다."));
		return FALSE;
	}

	// 기존 출력 이미지 삭제
	DeleteOutputImage();

	// 메모리 할당
	outputHeight = inputHeight;
	outputWidth = inputWidth;
	outputImage = Create2DArray(outputHeight, outputWidth, RGB(0, 0, 0));

	double** tempInputImageH = Create2DArray(inputHeight, inputWidth, 0.);
	double** tempInputImageS = Create2DArray(inputHeight, inputWidth, 0.);
	double** tempInputImageI = Create2DArray(inputHeight, inputWidth, 0.);
	double** tempOutputImageI = Create2DArray(outputHeight, outputWidth, 0.);

	// RGB -> HSI 변환
	for (int ypos = 0; ypos < inputHeight; ++ypos) {
		for (int xpos = 0; xpos < inputWidth; ++xpos) {
			double* hsi;
			hsi = RGB2HSI(inputImage[ypos][xpos]);

			tempInputImageH[ypos][xpos] = hsi[0];
			tempInputImageS[ypos][xpos] = hsi[1];
			tempInputImageI[ypos][xpos] = hsi[2];
			delete[] hsi;
		}
	}

	// 최소값, 최대값 계산
	double histMin = tempInputImageI[0][0], histMax = tempInputImageI[0][0];
	for (int ypos = 0; ypos < inputHeight; ++ypos) {
		for (int xpos = 0; xpos < inputWidth; ++xpos) {
			histMin = min(tempInputImageI[ypos][xpos], histMin);
			histMax = max(tempInputImageI[ypos][xpos], histMax);
		}
	}

	// 처리 (명도 스트레칭)
	double oldPx, newPx;
	for (int ypos = 0; ypos < outputHeight; ++ypos) {
		for (int xpos = 0; xpos < outputWidth; ++xpos) {
			oldPx = tempInputImageI[ypos][xpos];
			newPx = (oldPx - histMin) / (histMax - histMin) * 255;
			newPx = Clamp(newPx, 0., 255.);

			tempOutputImageI[ypos][xpos] = newPx;
		}
	}

	// HSI -> RGB 변환
	for (int ypos = 0; ypos < inputHeight; ++ypos) {
		for (int xpos = 0; xpos < inputWidth; ++xpos) {
			double h, s, i;
			h = tempInputImageH[ypos][xpos];
			s = tempInputImageS[ypos][xpos];
			i = tempOutputImageI[ypos][xpos];

			outputImage[ypos][xpos] = HSI2RGB(h, s, i);
		}
	}

	// 임시 메모리 할당 해제
	Delete2DArray(tempInputImageH, inputHeight);
	Delete2DArray(tempInputImageS, inputHeight);
	Delete2DArray(tempInputImageI, inputHeight);
	Delete2DArray(tempOutputImageI, outputHeight);

	return TRUE;
}


// 히스토그램 처리 - 엔드인 탐색
BOOL CColorImageProcessorDoc::ProcessHistEndin()
{
	// 입력 이미지 검사
	if (inputImage == NULL) {
		AfxMessageBox(_T("불러온 이미지가 없습니다."));
		return FALSE;
	}

	// 입력값 (0~100)
	CSingleConstDlg dialog((CWnd*)nullptr, _T("엔드인 탐색"), _T("0~100 사이의 정수 입력"), 0, 100);
	if (dialog.DoModal() != IDOK) {
		return FALSE;
	}
	int value = (int)dialog.value;

	// 기존 출력 이미지 삭제
	DeleteOutputImage();

	// 메모리 할당
	outputHeight = inputHeight;
	outputWidth = inputWidth;
	outputImage = Create2DArray(outputHeight, outputWidth, RGB(0, 0, 0));

	double** tempInputImageH = Create2DArray(inputHeight, inputWidth, 0.);
	double** tempInputImageS = Create2DArray(inputHeight, inputWidth, 0.);
	double** tempInputImageI = Create2DArray(inputHeight, inputWidth, 0.);
	double** tempOutputImageI = Create2DArray(outputHeight, outputWidth, 0.);

	// RGB -> HSI 변환
	for (int ypos = 0; ypos < inputHeight; ++ypos) {
		for (int xpos = 0; xpos < inputWidth; ++xpos) {
			double* hsi;
			hsi = RGB2HSI(inputImage[ypos][xpos]);

			tempInputImageH[ypos][xpos] = hsi[0];
			tempInputImageS[ypos][xpos] = hsi[1];
			tempInputImageI[ypos][xpos] = hsi[2];
			delete[] hsi;
		}
	}

	// 최소값, 최대값 계산
	double histMin = tempInputImageI[0][0], histMax = tempInputImageI[0][0];
	for (int ypos = 0; ypos < inputHeight; ++ypos) {
		for (int xpos = 0; xpos < inputWidth; ++xpos) {
			histMin = min(tempInputImageI[ypos][xpos], histMin);
			histMax = max(tempInputImageI[ypos][xpos], histMax);
		}
	}
	histMin += value;
	histMax -= value;


	// 처리 (명도 스트레칭)
	double oldPx, newPx;
	for (int ypos = 0; ypos < outputHeight; ++ypos) {
		for (int xpos = 0; xpos < outputWidth; ++xpos) {
			oldPx = tempInputImageI[ypos][xpos];
			newPx = (oldPx - histMin) / (histMax - histMin) * 255;
			if (oldPx <= histMin)
				newPx = 0;
			else if (oldPx >= histMax)
				newPx = 255;
			else
				newPx = Clamp(newPx, 0., 255.);

			tempOutputImageI[ypos][xpos] = newPx;
		}
	}

	// HSI -> RGB 변환
	for (int ypos = 0; ypos < inputHeight; ++ypos) {
		for (int xpos = 0; xpos < inputWidth; ++xpos) {
			double h, s, i;
			h = tempInputImageH[ypos][xpos];
			s = tempInputImageS[ypos][xpos];
			i = tempOutputImageI[ypos][xpos];

			outputImage[ypos][xpos] = HSI2RGB(h, s, i);
		}
	}

	// 임시 메모리 할당 해제
	Delete2DArray(tempInputImageH, inputHeight);
	Delete2DArray(tempInputImageS, inputHeight);
	Delete2DArray(tempInputImageI, inputHeight);
	Delete2DArray(tempOutputImageI, outputHeight);

	return TRUE;
}


// 히스토그램 처리 - 평활화(RGB)
BOOL CColorImageProcessorDoc::ProcessHistEqualizeRgb()
{
	// 입력 이미지 검사
	if (inputImage == NULL) {
		AfxMessageBox(_T("불러온 이미지가 없습니다."));
		return FALSE;
	}

	// 기존 출력 이미지 삭제
	DeleteOutputImage();

	// 메모리 할당
	outputHeight = inputHeight;
	outputWidth = inputWidth;
	outputImage = Create2DArray(outputHeight, outputWidth, RGB(0, 0, 0));

	// 히스토그램 계산
	int histArrayR[256] = { 0, };
	int histArrayG[256] = { 0, };
	int histArrayB[256] = { 0, };
	int sumArrayR[256] = { 0, };
	int sumArrayG[256] = { 0, };
	int sumArrayB[256] = { 0, };
	int inPx, histSum = 0;
	for (int ypos = 0; ypos < inputHeight; ++ypos) {
		for (int xpos = 0; xpos < inputWidth; ++xpos) {
			inPx = GetRValue(inputImage[ypos][xpos]);		// Red
			histArrayR[inPx] += 1;
			inPx = GetGValue(inputImage[ypos][xpos]);		// Green
			histArrayG[inPx] += 1;
			inPx = GetBValue(inputImage[ypos][xpos]);		// Blue
			histArrayB[inPx] += 1;
		}
	}
	// 누적 히스토그램 계산
	sumArrayR[0] = histArrayR[0];
	sumArrayG[0] = histArrayG[0];
	sumArrayB[0] = histArrayB[0];
	for (int i = 1; i < 256; ++i) {
		sumArrayR[i] = sumArrayR[i - 1] + histArrayR[i];
		sumArrayG[i] = sumArrayG[i - 1] + histArrayG[i];
		sumArrayB[i] = sumArrayB[i - 1] + histArrayB[i];
	}

	// 정규화
	int normalArrayR[256];
	int normalArrayG[256];
	int normalArrayB[256];
	for (int i = 0; i < 256; ++i) {
		normalArrayR[i] = sumArrayR[i] * 255 / (inputHeight * inputWidth);
		normalArrayG[i] = sumArrayG[i] * 255 / (inputHeight * inputWidth);
		normalArrayB[i] = sumArrayB[i] * 255 / (inputHeight * inputWidth);
	}

	// 처리
	COLORREF px;
	int outR, outG, outB;
	for (int ypos = 0; ypos < outputHeight; ++ypos) {
		for (int xpos = 0; xpos < outputWidth; ++xpos) {
			px = inputImage[ypos][xpos];
			outR = Clamp(normalArrayR[GetRValue(px)], 0, 255);
			outG = Clamp(normalArrayG[GetGValue(px)], 0, 255);
			outB = Clamp(normalArrayB[GetBValue(px)], 0, 255);

			outputImage[ypos][xpos] = RGB(outR, outG, outB);
		}
	}

	return TRUE;
}


// 히스토그램 처리 - 평활화(HSI)
BOOL CColorImageProcessorDoc::ProcessHistEqualizeHsi()
{
	// 입력 이미지 검사
	if (inputImage == NULL) {
		AfxMessageBox(_T("불러온 이미지가 없습니다."));
		return FALSE;
	}

	// 기존 출력 이미지 삭제
	DeleteOutputImage();

	// 메모리 할당
	outputHeight = inputHeight;
	outputWidth = inputWidth;
	outputImage = Create2DArray(outputHeight, outputWidth, RGB(0, 0, 0));

	double** tempInputImageH = Create2DArray(inputHeight, inputWidth, 0.);
	double** tempInputImageS = Create2DArray(inputHeight, inputWidth, 0.);
	double** tempInputImageI = Create2DArray(inputHeight, inputWidth, 0.);
	double** tempOutputImageI = Create2DArray(outputHeight, outputWidth, 0.);

	// RGB -> HSI 변환
	for (int ypos = 0; ypos < inputHeight; ++ypos) {
		for (int xpos = 0; xpos < inputWidth; ++xpos) {
			double* hsi;
			hsi = RGB2HSI(inputImage[ypos][xpos]);

			tempInputImageH[ypos][xpos] = hsi[0];
			tempInputImageS[ypos][xpos] = hsi[1];
			tempInputImageI[ypos][xpos] = hsi[2];
			delete[] hsi;
		}
	}

	// 히스토그램 계산
	int histArray[256] = { 0, };
	int sumArray[256] = { 0, };
	int inPx, histSum = 0;
	for (int ypos = 0; ypos < inputHeight; ++ypos) {
		for (int xpos = 0; xpos < inputWidth; ++xpos) {
			inPx = (int)tempInputImageI[ypos][xpos];
			histArray[inPx] += 1;
		}
	}
	// 누적 히스토그램 계산
	for (int i = 0; i < 256; ++i) {
		histSum += histArray[i];
		sumArray[i] = histSum;
	}

	// 정규화
	int normalArray[256];
	for (int i = 0; i < 256; ++i) {
		normalArray[i] = sumArray[i] * 255 / (inputHeight * inputWidth);
	}

	// 처리 (명도 평활화)
	int outPx;
	for (int ypos = 0; ypos < outputHeight; ++ypos) {
		for (int xpos = 0; xpos < outputWidth; ++xpos) {
			inPx = (int)tempInputImageI[ypos][xpos];
			outPx = Clamp(normalArray[inPx], 0, 255);

			tempOutputImageI[ypos][xpos] = (unsigned char)outPx;
		}
	}

	// HSI -> RGB 변환
	for (int ypos = 0; ypos < inputHeight; ++ypos) {
		for (int xpos = 0; xpos < inputWidth; ++xpos) {
			double h, s, i;
			h = tempInputImageH[ypos][xpos];
			s = tempInputImageS[ypos][xpos];
			i = tempOutputImageI[ypos][xpos];

			outputImage[ypos][xpos] = HSI2RGB(h, s, i);
		}
	}

	// 임시 메모리 할당 해제
	Delete2DArray(tempInputImageH, inputHeight);
	Delete2DArray(tempInputImageS, inputHeight);
	Delete2DArray(tempInputImageI, inputHeight);
	Delete2DArray(tempOutputImageI, outputHeight);

	return TRUE;
}


// 영역 처리 - 엠보싱 (RGB)
BOOL CColorImageProcessorDoc::ProcessAreaEmboss()
{
	// 입력 이미지 검사
	if (inputImage == NULL) {
		AfxMessageBox(_T("불러온 이미지가 없습니다."));
		return FALSE;
	}

	// 기존 출력 이미지 삭제
	DeleteOutputImage();

	// 메모리 할당
	outputHeight = inputHeight;
	outputWidth = inputWidth;
	outputImage = Create2DArray(outputHeight, outputWidth, RGB(0, 0, 0));

	// 마스크 적용
	double mask[3][3] = { {-1,0,0},{0,0,0},{0,0,1} };
	COLORREF** tempOutputImage = CreateMaskedImage(mask, inputImage, 127.);

	// 처리
	for (int ypos = 0; ypos < outputHeight; ++ypos) {
		for (int xpos = 0; xpos < outputWidth; ++xpos) {
			outputImage[ypos][xpos] = tempOutputImage[ypos][xpos];
		}
	}

	Delete2DArray(tempOutputImage, outputHeight);

	return TRUE;
}


// 영역 처리 - 엠보싱 HSI
BOOL CColorImageProcessorDoc::ProcessAreaEmbossHsi()
{
	// 입력 이미지 검사
	if (inputImage == NULL) {
		AfxMessageBox(_T("불러온 이미지가 없습니다."));
		return FALSE;
	}

	// 기존 출력 이미지 삭제
	DeleteOutputImage();

	// 메모리 할당
	outputHeight = inputHeight;
	outputWidth = inputWidth;
	outputImage = Create2DArray(outputHeight, outputWidth, RGB(0, 0, 0));

	// RGB -> HSI 변환
	double** tempInputImageH = Create2DArray(inputHeight + 2, inputWidth + 2, 0.);
	double** tempInputImageS = Create2DArray(inputHeight + 2, inputWidth + 2, 0.);
	double** tempInputImageI = Create2DArray(inputHeight + 2, inputWidth + 2, 0.);

	for (int ypos = 0; ypos < inputHeight; ++ypos) {
		for (int xpos = 0; xpos < inputWidth; ++xpos) {
			double* hsi;
			unsigned char r, g, b;
			r = GetRValue(inputImage[ypos][xpos]);
			g = GetGValue(inputImage[ypos][xpos]);
			b = GetBValue(inputImage[ypos][xpos]);
			hsi = RGB2HSI(r, g, b);

			tempInputImageH[ypos][xpos] = hsi[0];
			tempInputImageS[ypos][xpos] = hsi[1];
			tempInputImageI[ypos][xpos] = hsi[2];
			delete[] hsi;
		}
	}

	// 마스크 적용 후 처리
	double mask[3][3] = { {-1,0,0},{0,0,0},{0,0,1} };
	double** tempOutputImageI = CreateMaskedImage(mask, tempInputImageI);

	// HSI -> RGB 변환
	for (int ypos = 0; ypos < inputHeight; ++ypos) {
		for (int xpos = 0; xpos < inputWidth; ++xpos) {
			double h, s, i;
			h = tempInputImageH[ypos][xpos];
			s = tempInputImageS[ypos][xpos];
			i = Clamp(tempOutputImageI[ypos][xpos] + 127, 0., 255.);

			outputImage[ypos][xpos] = HSI2RGB(h, s, i);
		}
	}

	Delete2DArray(tempInputImageH, inputHeight + 2);
	Delete2DArray(tempInputImageS, inputHeight + 2);
	Delete2DArray(tempInputImageI, inputHeight + 2);
	Delete2DArray(tempOutputImageI, outputHeight);

	return TRUE;
}


// 영역 처리 - 샤프닝
BOOL CColorImageProcessorDoc::ProcessAreaSharp()
{
	// 입력 이미지 검사
	if (inputImage == NULL) {
		AfxMessageBox(_T("불러온 이미지가 없습니다."));
		return FALSE;
	}

	// 입력값
	CSingleConstDlg dialog((CWnd*)nullptr, _T("샤프닝"), _T("강도 (1이상 정수)"), 1, 9999999);
	if (dialog.DoModal() != IDOK) {
		return FALSE;
	}
	int strength = (int)dialog.value;

	// 기존 출력 이미지 삭제
	DeleteOutputImage();

	// 메모리 할당
	outputHeight = inputHeight;
	outputWidth = inputWidth;
	outputImage = Create2DArray(outputHeight, outputWidth, RGB(0, 0, 0));

	// 마스크 적용
	double b = (1 - strength) / 8.;
	double mask[3][3] = { {b,b,b},{b,(double)strength,b},{b,b,b} };
	COLORREF** tempOutputImage = CreateMaskedImage(mask, inputImage);

	// 처리
	for (int ypos = 0; ypos < outputHeight; ++ypos) {
		for (int xpos = 0; xpos < outputWidth; ++xpos) {
			outputImage[ypos][xpos] = tempOutputImage[ypos][xpos];
		}
	}

	Delete2DArray(tempOutputImage, outputHeight);

	return TRUE;
}


// 영역 처리 - 블러링
BOOL CColorImageProcessorDoc::ProcessAreaBlur()
{
	// 입력 이미지 검사
	if (inputImage == NULL) {
		AfxMessageBox(_T("불러온 이미지가 없습니다."));
		return FALSE;
	}

	CSingleConstDlg dialog((CWnd*)nullptr, _T("블러링"), _T("강도 (1이상 정수)"), 1, 9999999);
	if (dialog.DoModal() != IDOK) {
		return FALSE;
	}
	int size = (int)dialog.value;
	size = size / 2 + 1;

	// 기존 출력 이미지 삭제
	DeleteOutputImage();

	// 메모리 할당
	outputHeight = inputHeight;
	outputWidth = inputWidth;
	outputImage = Create2DArray(outputHeight, outputWidth, RGB(0, 0, 0));

	// 마스크 적용
	double** mask = Create2DArray(size, size, 1. / (size * size));
	COLORREF** tempOutputImage = CreateMaskedImage(mask, size, inputImage);

	// 처리
	for (int ypos = 0; ypos < outputHeight; ++ypos) {
		for (int xpos = 0; xpos < outputWidth; ++xpos) {
			outputImage[ypos][xpos] = tempOutputImage[ypos][xpos];
		}
	}

	Delete2DArray(tempOutputImage, outputHeight);

	return TRUE;
}


// 영역 처리 - 경계선 검출
BOOL CColorImageProcessorDoc::ProcessAreaEdge()
{
	// 입력 이미지 검사
	if (inputImage == NULL) {
		AfxMessageBox(_T("불러온 이미지가 없습니다."));
		return FALSE;
	}

	// 입력값
	CStringArray items;
	items.Add(_T("수직 경계 검출"));
	items.Add(_T("수평 경계 검출"));
	items.Add(_T("수직-수평 경계 검출"));
	items.Add(_T("로버츠 행 검출"));
	items.Add(_T("로버츠 열 검출"));
	items.Add(_T("프리윗 행 검출"));
	items.Add(_T("프리윗 열 검출"));
	items.Add(_T("소벨 행 검출"));
	items.Add(_T("소벨 열 검출"));
	items.Add(_T("라플라시안 마스크 1"));
	items.Add(_T("라플라시안 마스크 2"));
	items.Add(_T("라플라시안 마스크 3"));

	CComboDlg dialog((CWnd*)nullptr, _T("경계선 검출"), _T("검출 마스크"), items);
	if (dialog.DoModal() != IDOK) {
		return FALSE;
	}
	int value = dialog.selection;

	// 기존 출력 이미지 삭제
	DeleteOutputImage();

	// 메모리 할당
	outputHeight = inputHeight;
	outputWidth = inputWidth;
	outputImage = Create2DArray(outputHeight, outputWidth, RGB(0, 0, 0));

	COLORREF** tempInputImage = CreateGrayscaleImage();
	COLORREF** tempOutputImage;

	// 마스크
	double mask0[3][3] = {						// 수직 엣지 검출 마스크
		{ 0, 0, 0 },
		{ -1, 1, 0 },
		{ 0, 0, 0 }
	};
	double mask1[3][3] = {						// 수평 엣지 검출 마스크
		{ 0, -1, 0 },
		{ 0, 1, 0 },
		{ 0, 0, 0 }
	};
	double mask2[3][3] = {						// 수직 엣지 + 수평 엣지
		{ 0, -1, 0 },
		{ -1, 2, 0 },
		{ 0, 0, 0 }
	};
	double mask3[3][3] = {						// 로버츠 행 검출 마스크
		{ -1, 0, 0 },
		{ 0, 1, 0 },
		{ 0, 0, 0 }
	};
	double mask4[3][3] = {						// 로버츠 열 검출 마스크
		{ 0, 0, -1 },
		{ 0, 1, 0 },
		{ 0, 0, 0 }
	};
	double mask5[3][3] = {						// 프리윗 행 검출 마스크
		{ -1, -1, -1 },
		{ 0, 0, 0 },
		{ 1, 1, 1 }
	};
	double mask6[3][3] = {						// 프리윗 열 검출 마스크
		{ 1, 0, -1 },
		{ 1, 0, -1 },
		{ 1, 0, -1 }
	};
	double mask7[3][3] = {						// 소벨 행 검출 마스크
		{ -1, -2, -1 },
		{ 0, 0 , 0 },
		{ 1, 2, 1 }
	};
	double mask8[3][3] = {						// 소벨 열 검출 마스크
		{ 1, 0, -1 },
		{ 2, 0, -2 },
		{ 1, 0, -1 }
	};
	double mask9[3][3] = {						// 라플라시안 마스크 1
		{ 0, -1, 0 },
		{ -1, 4, -1 },
		{ 0, -1, 0 }
	};
	double mask10[3][3] = {						// 라플라시안 마스크 2
		{ 1, 1, 1 },
		{ 1, -8, 1 },
		{ 1, 1, 1 }
	};
	double mask11[3][3] = {						// 라플라시안 마스크 3
		{ -1, -1, -1 },
		{ -1, 8, -1 },
		{ -1, -1, -1 }
	};

	// 마스크 적용
	switch (value) {
	case 0:
		tempOutputImage = CreateMaskedImage(mask0, tempInputImage);
		break;
	case 1:
		tempOutputImage = CreateMaskedImage(mask1, tempInputImage);
		break;
	case 2:
		tempOutputImage = CreateMaskedImage(mask2, tempInputImage);
		break;
	case 3:
		tempOutputImage = CreateMaskedImage(mask3, tempInputImage);
		break;
	case 4:
		tempOutputImage = CreateMaskedImage(mask4, tempInputImage);
		break;
	case 5:
		tempOutputImage = CreateMaskedImage(mask5, tempInputImage);
		break;
	case 6:
		tempOutputImage = CreateMaskedImage(mask6, tempInputImage);
		break;
	case 7:
		tempOutputImage = CreateMaskedImage(mask7, tempInputImage);
		break;
	case 8:
		tempOutputImage = CreateMaskedImage(mask8, tempInputImage);
		break;
	case 9:
		tempOutputImage = CreateMaskedImage(mask9, tempInputImage);
		break;
	case 10:
		tempOutputImage = CreateMaskedImage(mask10, tempInputImage);
		break;
	case 11:
		tempOutputImage = CreateMaskedImage(mask11, tempInputImage);
		break;
	default:
		AfxMessageBox(_T("잘못된 파라미터 값"));
		return FALSE;
	}

	// 처리
	for (int ypos = 0; ypos < outputHeight; ++ypos) {
		for (int xpos = 0; xpos < outputWidth; ++xpos) {
			outputImage[ypos][xpos] = tempOutputImage[ypos][xpos];
		}
	}

	Delete2DArray(tempInputImage, inputHeight);
	Delete2DArray(tempOutputImage, outputHeight);

	return TRUE;
}


// 색상 처리 - 색조
BOOL CColorImageProcessorDoc::ProcessColorHue()
{
	// 입력 이미지 검사
	if (inputImage == NULL) {
		AfxMessageBox(_T("불러온 이미지가 없습니다."));
		return FALSE;
	}

	// 입력값
	CSingleConstDlg dialog((CWnd*)nullptr, _T("색조 이동"), _T("-360~360 사이 실수 입력"), -360, 360);
	if (dialog.DoModal() != IDOK) {
		return FALSE;
	}
	double value = dialog.value;

	// 기존 출력 이미지 삭제
	DeleteOutputImage();

	// 메모리 할당
	outputHeight = inputHeight;
	outputWidth = inputWidth;
	outputImage = Create2DArray(outputHeight, outputWidth, RGB(0, 0, 0));

	// 처리
	for (int ypos = 0; ypos < outputHeight; ++ypos) {
		for (int xpos = 0; xpos < outputWidth; ++xpos) {
			double h, s, i;

			double* hsi = RGB2HSI(inputImage[ypos][xpos]);
			h = hsi[0];
			s = hsi[1];
			i = hsi[2];
			delete[] hsi;

			if (std::isnan(h)) {						// Grayscale의 경우 Hue값이 NaN이 나옴
				outputImage[ypos][xpos] = inputImage[ypos][xpos];
			}
			else {
				h = HUE(h);										// 2PI -> 360
				h = h + value;
				if (h < 0)
					h = 360. + h;
				else if (h > 360)
					h = h - 360.;
				h = (h / 360.) * (2 * M_PI);	// 360 -> 2PI

				outputImage[ypos][xpos] = HSI2RGB(h, s, i);
			}
		}
	}

	return TRUE;
}


// 색상 처리 - 채도
BOOL CColorImageProcessorDoc::ProcessColorSaturation()
{
	// 입력 이미지 검사
	if (inputImage == NULL) {
		AfxMessageBox(_T("불러온 이미지가 없습니다."));
		return FALSE;
	}

	// 입력값
	CSingleConstDlg dialog((CWnd*)nullptr, _T("채도"), _T("-100~100 사이 정수 입력"), -100, 100);
	if (dialog.DoModal() != IDOK) {
		return FALSE;
	}
	double value = floor(dialog.value) / 100;

	// 기존 출력 이미지 삭제
	DeleteOutputImage();

	// 메모리 할당
	outputHeight = inputHeight;
	outputWidth = inputWidth;
	outputImage = Create2DArray(outputHeight, outputWidth, RGB(0, 0, 0));

	// 처리
	for (int ypos = 0; ypos < outputHeight; ++ypos) {
		for (int xpos = 0; xpos < outputWidth; ++xpos) {
			// HIS -> H(색상 0~360), S(채도 0.0~1.0), I(명도 0~255)
			double h, s, i;

			double* hsi = RGB2HSI(inputImage[ypos][xpos]);
			h = hsi[0];
			s = hsi[1];
			i = hsi[2];
			delete[] hsi;

			s = Clamp(s + value, 0.0, 1.0);

			outputImage[ypos][xpos] = HSI2RGB(h, s, i);
		}
	}

	return TRUE;
}


// 색상 처리 - 색상 선택
BOOL CColorImageProcessorDoc::ProcessColorPicker()
{
	// 입력 이미지 검사
	if (inputImage == NULL) {
		AfxMessageBox(_T("불러온 이미지가 없습니다."));
		return FALSE;
	}

	// 입력값
	CColorConstDlg dialog((CWnd*)nullptr, _T("색상 선택"), _T("색상"), _T("선택 범위 (0~100)"), 0, 100);
	if (dialog.DoModal() != IDOK) {
		return FALSE;
	}
	COLORREF color = dialog.m_color.GetColor();
	double value = (dialog.value / 100) * M_PI;

	// 기존 출력 이미지 삭제
	DeleteOutputImage();

	// 메모리 할당
	outputHeight = inputHeight;
	outputWidth = inputWidth;
	outputImage = Create2DArray(outputHeight, outputWidth, RGB(0, 0, 0));

	// 감지할 색상 범위
	double* hsi = RGB2HSI(color);

	double target = hsi[0];
	double start = target - value;
	double end = target + value;

	delete[] hsi;

	// 처리
	for (int ypos = 0; ypos < outputHeight; ++ypos) {
		for (int xpos = 0; xpos < outputWidth; ++xpos) {
			hsi = RGB2HSI(inputImage[ypos][xpos]);
			double hue = hsi[0];
			delete[] hsi;

			double h = hue;
			if (start < 0) {
				h = hue > M_PI ? hue - M_PI : hue;
			}
			if (end > 2 * M_PI) {
				h = hue < M_PI ? hue + M_PI : hue;
			}

			if (start <= h && h <= end) {
				outputImage[ypos][xpos] = inputImage[ypos][xpos];
			}
			else {
				double outPx = (GetRValue(inputImage[ypos][xpos]) + GetGValue(inputImage[ypos][xpos]) + GetBValue(inputImage[ypos][xpos])) / 3.;
				outPx = Clamp(outPx, 0., 255.);
				outputImage[ypos][xpos] = RGB((int)outPx, (int)outPx, (int)outPx);
			}
		}
	}

	return TRUE;
}


// 출력 이미지를 입력 이미지로 복사
BOOL CColorImageProcessorDoc::CopyOutputToInput()
{
	DeleteInputImage();

	inputHeight = outputHeight;
	inputWidth = outputWidth;
	inputImage = Create2DArray(inputHeight, inputWidth, RGB(0, 0, 0));

	for (int ypos = 0; ypos < outputHeight; ++ypos) {
		for (int xpos = 0; xpos < outputWidth; ++xpos) {
			inputImage[ypos][xpos] = outputImage[ypos][xpos];
		}
	}

	return TRUE;
}


// 출력 이미지를 보조 이미지로 복사
BOOL CColorImageProcessorDoc::CopyOutputToSub()
{
	DeleteSubImage();

	subHeight = outputHeight;
	subWidth = outputWidth;
	subImage = Create2DArray(subHeight, subWidth, RGB(0, 0, 0));

	for (int ypos = 0; ypos < outputHeight; ++ypos) {
		for (int xpos = 0; xpos < outputWidth; ++xpos) {
			subImage[ypos][xpos] = outputImage[ypos][xpos];
		}
	}

	return TRUE;
}

