
// Color Image ProcessorView.cpp: CColorImageProcessorView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Color Image Processor.h"
#endif

#include "Color Image ProcessorDoc.h"
#include "Color Image ProcessorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CColorImageProcessorView

IMPLEMENT_DYNCREATE(CColorImageProcessorView, CView)

BEGIN_MESSAGE_MAP(CColorImageProcessorView, CView)
	ON_COMMAND(IDM_POINT_EQUAL, &CColorImageProcessorView::OnPointEqual)
	ON_COMMAND(IDM_POINT_GRAYSCALE, &CColorImageProcessorView::OnPointGrayscale)
	ON_COMMAND(IDM_POINT_INVERT, &CColorImageProcessorView::OnPointInvert)
	ON_COMMAND(IDM_POINT_THRESHOLD, &CColorImageProcessorView::OnPointThreshold)
	ON_COMMAND(IDM_GEOMETRY_ZOOMIN, &CColorImageProcessorView::OnGeometryZoomin)
	ON_COMMAND(IDM_GEOMETRY_ZOOMOUT, &CColorImageProcessorView::OnGeometryZoomout)
	ON_COMMAND(IDM_AREA_EMBOSS, &CColorImageProcessorView::OnAreaEmboss)
	ON_COMMAND(IDM_COLOR_SATURATION, &CColorImageProcessorView::OnColorSaturation)
	ON_COMMAND(IDM_COLOR_PICKER, &CColorImageProcessorView::OnColorPicker)
	ON_COMMAND(IDM_MIRROR_HORI, &CColorImageProcessorView::OnMirrorHori)
	ON_COMMAND(IDM_MIRROR_VERT, &CColorImageProcessorView::OnMirrorVert)
	ON_COMMAND(IDM_GEOMETRY_TRANSLATE, &CColorImageProcessorView::OnGeometryTranslate)
	ON_COMMAND(IDM_GEOMETRY_ROTATE, &CColorImageProcessorView::OnGeometryRotate)
	ON_COMMAND(IDM_AREA_SHARP, &CColorImageProcessorView::OnAreaSharp)
	ON_COMMAND(IDM_AREA_BLUR, &CColorImageProcessorView::OnAreaBlur)
	ON_COMMAND(IDM_AREA_EMBOSS_HSI, &CColorImageProcessorView::OnAreaEmbossHsi)
	ON_COMMAND(IDM_POINT_GAMMA, &CColorImageProcessorView::OnPointGamma)
	ON_COMMAND(IDM_POINT_CONTRAST, &CColorImageProcessorView::OnPointContrast)
	ON_COMMAND(IDM_HIST_EQUALIZE_RGB, &CColorImageProcessorView::OnHistEqualizeRgb)
	ON_COMMAND(IDM_HIST_EQUALIZE_HSI, &CColorImageProcessorView::OnHistEqualizeHsi)
	ON_COMMAND(IDM_POINT_POSTERIZE, &CColorImageProcessorView::OnPointPosterize)
	ON_COMMAND(IDM_HIST_STRETCH, &CColorImageProcessorView::OnHistStretch)
	ON_COMMAND(IDM_HIST_ENDIN, &CColorImageProcessorView::OnHistEndin)
	ON_COMMAND(IDM_AREA_EDGE, &CColorImageProcessorView::OnAreaEdge)
	ON_COMMAND(IDM_POINT_BRIGHTNESS, &CColorImageProcessorView::OnPointBrightness)
	ON_COMMAND(IDM_SUBFILE_OPEN, &CColorImageProcessorView::OnSubfileOpen)
	ON_COMMAND(IDM_OUT_TO_IN, &CColorImageProcessorView::OnOutToIn)
	ON_COMMAND(IDM_OUT_TO_SUB, &CColorImageProcessorView::OnOutToSub)
	ON_COMMAND(IDM_LOGIC_AND, &CColorImageProcessorView::OnLogicAnd)
	ON_COMMAND(IDM_LOGIC_OR, &CColorImageProcessorView::OnLogicOr)
	ON_COMMAND(IDM_LOGIC_XOR, &CColorImageProcessorView::OnLogicXor)
	ON_COMMAND(IDM_MATH_ADD, &CColorImageProcessorView::OnMathAdd)
	ON_COMMAND(IDM_MATH_SUBTRACT, &CColorImageProcessorView::OnMathSubtract)
	ON_COMMAND(IDM_MATH_MULTIPLY, &CColorImageProcessorView::OnMathMultiply)
	ON_COMMAND(IDM_MATH_DIVIDE, &CColorImageProcessorView::OnMathDivide)
	ON_COMMAND(IDM_GEOMETRY_BLEND, &CColorImageProcessorView::OnGeometryBlend)
	ON_COMMAND(IDM_COLOR_HUE, &CColorImageProcessorView::OnColorHue)
END_MESSAGE_MAP()

// CColorImageProcessorView 생성/소멸

CColorImageProcessorView::CColorImageProcessorView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CColorImageProcessorView::~CColorImageProcessorView()
{
}

BOOL CColorImageProcessorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CColorImageProcessorView 그리기

void CColorImageProcessorView::OnDraw(CDC* pDC)
{
	CColorImageProcessorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// 화면 크기
	CRect rect;
	pDC->GetClipBox(&rect);
	double MAXSIZE = rect.Height() - 30;

	CDC memDC;
	CBitmap bitmap;
	CBitmap* pOldBitmap;
	double hop;

	/*** 입력 이미지 ***/

	if (pDoc->inputImage != NULL)
		pDC->TextOutW(5, 5, _T("Main"));

	// 비트맵 메모리 생성
	memDC.CreateCompatibleDC(pDC);
	bitmap.CreateCompatibleBitmap(pDC, pDoc->inputWidth, pDoc->inputHeight);
	pOldBitmap = (CBitmap*)memDC.SelectObject(&bitmap);
	memDC.PatBlt(0, 0, pDoc->inputWidth, pDoc->inputHeight, WHITENESS);

	// 이미지 크기 줄이기
	int tempInputHeight = pDoc->inputHeight;
	int tempInputWidth = pDoc->inputWidth;
	hop = 1.0;

	if (tempInputHeight > MAXSIZE || tempInputWidth > MAXSIZE) {
		if (tempInputWidth > tempInputHeight) {
			hop = tempInputWidth / MAXSIZE;
		}
		else {
			hop = tempInputHeight / MAXSIZE;
		}
		tempInputHeight = (int)(tempInputHeight / hop);
		tempInputWidth = (int)(tempInputWidth / hop);
	}

	// 메모리에 이미지 기록
	for (int ypos = 0; ypos < tempInputHeight; ++ypos) {
		for (int xpos = 0; xpos < tempInputWidth; ++xpos) {
			memDC.SetPixel(xpos, ypos, pDoc->inputImage[(int)(ypos * hop)][(int)(xpos * hop)]);
		}
	}

	// 메모리 상의 이미지를 화면에 출력
	pDC->BitBlt(5, 25, pDoc->inputWidth, pDoc->inputHeight, &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	bitmap.DeleteObject();

	/*** 보조 이미지 ***/

	if (pDoc->subImage != NULL)
		pDC->TextOutW(10 + tempInputWidth, 5, _T("Sub"));

	// 비트맵 메모리 생성
	memDC.CreateCompatibleDC(pDC);
	bitmap.CreateCompatibleBitmap(pDC, pDoc->subWidth, pDoc->subHeight);
	pOldBitmap = (CBitmap*)memDC.SelectObject(&bitmap);
	memDC.PatBlt(0, 0, pDoc->subWidth, pDoc->subHeight, WHITENESS);

	// 이미지 크기 줄이기
	int tempSubHeight = pDoc->subHeight;
	int tempSubWidth = pDoc->subWidth;
	hop = 1.0;

	if (tempSubHeight > MAXSIZE || tempSubWidth > MAXSIZE) {
		if (tempSubWidth > tempSubHeight) {
			hop = tempSubWidth / MAXSIZE;
		}
		else {
			hop = tempSubHeight / MAXSIZE;
		}
		tempSubHeight = (int)(tempSubHeight / hop);
		tempSubWidth = (int)(tempSubWidth / hop);
	}

	// 출력 이미지 그리기
	for (int ypos = 0; ypos < tempSubHeight; ++ypos) {
		for (int xpos = 0; xpos < tempSubWidth; ++xpos) {
			memDC.SetPixel(xpos, ypos, pDoc->subImage[(int)(ypos * hop)][(int)(xpos * hop)]);
		}
	}

	// 메모리 상의 이미지를 화면에 출력
	pDC->BitBlt(10 + tempInputWidth, 25, pDoc->subWidth, pDoc->subHeight, &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	bitmap.DeleteObject();

	/*** 출력 이미지 ***/

	if (pDoc->outputImage != NULL)
		pDC->TextOutW(15 + tempInputWidth + tempSubWidth, 5, _T("Output"));

	// 비트맵 메모리 생성
	memDC.CreateCompatibleDC(pDC);
	bitmap.CreateCompatibleBitmap(pDC, pDoc->outputWidth, pDoc->outputHeight);
	pOldBitmap = (CBitmap*)memDC.SelectObject(&bitmap);
	memDC.PatBlt(0, 0, pDoc->outputWidth, pDoc->outputHeight, WHITENESS);

	// 이미지 크기 줄이기
	int tempOutputHeight = pDoc->outputHeight;
	int tempOutputWidth = pDoc->outputWidth;
	hop = 1.0;

	if (tempOutputHeight > MAXSIZE || tempOutputWidth > MAXSIZE) {
		if (tempOutputWidth > tempOutputHeight) {
			hop = tempOutputWidth / MAXSIZE;
		}
		else {
			hop = tempOutputHeight / MAXSIZE;
		}
		tempOutputHeight = (int)(tempOutputHeight / hop);
		tempOutputWidth = (int)(tempOutputWidth / hop);
	}

	// 출력 이미지 그리기
	for (int ypos = 0; ypos < tempOutputHeight; ++ypos) {
		for (int xpos = 0; xpos < tempOutputWidth; ++xpos) {
			memDC.SetPixel(xpos, ypos, pDoc->outputImage[(int)(ypos * hop)][(int)(xpos * hop)]);
		}
	}

	// 메모리 상의 이미지를 화면에 출력
	pDC->BitBlt(15 + tempInputWidth + tempSubWidth, 25, pDoc->outputWidth, pDoc->outputHeight, &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	bitmap.DeleteObject();
}


// CColorImageProcessorView 진단

#ifdef _DEBUG
void CColorImageProcessorView::AssertValid() const
{
	CView::AssertValid();
}

void CColorImageProcessorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CColorImageProcessorDoc* CColorImageProcessorView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CColorImageProcessorDoc)));
	return (CColorImageProcessorDoc*)m_pDocument;
}
#endif //_DEBUG


// CColorImageProcessorView 메시지 처리기

// 보조 이미지 열기
void CColorImageProcessorView::OnSubfileOpen()
{
	CColorImageProcessorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDoc->OpenSubfile())
		Invalidate(TRUE);
}


// 화소점 처리 - 동일
void CColorImageProcessorView::OnPointEqual()
{
	CColorImageProcessorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDoc->ProcessPointEqual())
		Invalidate(TRUE);
}


// 화소점 처리 - 그레이스케일
void CColorImageProcessorView::OnPointGrayscale()
{
	CColorImageProcessorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDoc->ProcessPointGrayscale())
		Invalidate(TRUE);
}


// 화소점 처리 - 밝기
void CColorImageProcessorView::OnPointBrightness()
{
	CColorImageProcessorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDoc->ProcessPointBrightness())
		Invalidate(TRUE);
}


// 화소점 처리 - 반전
void CColorImageProcessorView::OnPointInvert()
{
	CColorImageProcessorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDoc->ProcessPointInvert())
		Invalidate(TRUE);
}


// 화소점 처리 - 흑백
void CColorImageProcessorView::OnPointThreshold()
{
	CColorImageProcessorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDoc->ProcessPointThreshold())
		Invalidate(TRUE);
}


// 화소점 처리 - 감마
void CColorImageProcessorView::OnPointGamma()
{
	CColorImageProcessorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDoc->ProcessPointGamma())
		Invalidate(TRUE);
}


// 화소점 처리 - 대비
void CColorImageProcessorView::OnPointContrast()
{
	CColorImageProcessorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDoc->ProcessPointContrast())
		Invalidate(TRUE);
}


// 화소점 처리 - 포스터라이즈
void CColorImageProcessorView::OnPointPosterize()
{
	CColorImageProcessorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDoc->ProcessPointPosterize())
		Invalidate(TRUE);
}


// 화소점 처리 - 산술 연산(더하기)
void CColorImageProcessorView::OnMathAdd()
{
	CColorImageProcessorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDoc->ProcessPointMath(0))
		Invalidate(TRUE);
}


// 화소점 처리 - 산술 연산(빼기)
void CColorImageProcessorView::OnMathSubtract()
{
	CColorImageProcessorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDoc->ProcessPointMath(1))
		Invalidate(TRUE);
}


// 화소점 처리 - 산술 연산(곱하기)
void CColorImageProcessorView::OnMathMultiply()
{
	CColorImageProcessorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDoc->ProcessPointMath(2))
		Invalidate(TRUE);
}


// 화소점 처리 - 산술 연산(나누기)
void CColorImageProcessorView::OnMathDivide()
{
	CColorImageProcessorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDoc->ProcessPointMath(3))
		Invalidate(TRUE);
}


// 화소점 처리 - 논리 연산(AND)
void CColorImageProcessorView::OnLogicAnd()
{
	CColorImageProcessorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDoc->ProcessPointLogic(0))
		Invalidate(TRUE);
}


// 화소점 처리 - 논리 연산(OR)
void CColorImageProcessorView::OnLogicOr()
{
	CColorImageProcessorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDoc->ProcessPointLogic(1))
		Invalidate(TRUE);
}


// 화소점 처리 - 논리 연산(XOR)
void CColorImageProcessorView::OnLogicXor()
{
	CColorImageProcessorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDoc->ProcessPointLogic(2))
		Invalidate(TRUE);
}


// 기하학 처리 - 확대
void CColorImageProcessorView::OnGeometryZoomin()
{
	CColorImageProcessorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDoc->ProcessGeometryZoomin())
		Invalidate(TRUE);
}


// 기하학 처리 - 축소
void CColorImageProcessorView::OnGeometryZoomout()
{
	CColorImageProcessorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDoc->ProcessGeometryZoomout())
		Invalidate(TRUE);
}


// 기하학 처리 - 회전
void CColorImageProcessorView::OnGeometryRotate()
{
	CColorImageProcessorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDoc->ProcessGeometryRotate())
		Invalidate(TRUE);
}


// 기하학 처리 - 이동
void CColorImageProcessorView::OnGeometryTranslate()
{
	CColorImageProcessorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDoc->ProcessGeometryTranslate())
		Invalidate(TRUE);
}


// 기하학 처리 - 좌우 대칭
void CColorImageProcessorView::OnMirrorHori()
{
	CColorImageProcessorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDoc->ProcessGeometryMirror(false))
		Invalidate(TRUE);
}


// 기하학 처리 - 상하 대칭
void CColorImageProcessorView::OnMirrorVert()
{
	CColorImageProcessorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDoc->ProcessGeometryMirror(true))
		Invalidate(TRUE);
}


// 기하학 처리 - 모핑
void CColorImageProcessorView::OnGeometryBlend()
{
	CColorImageProcessorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDoc->ProcessGeometryBlend())
		Invalidate(TRUE);
}


// 히스토그램 처리 - 히스토그램 스트레칭
void CColorImageProcessorView::OnHistStretch()
{
	CColorImageProcessorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDoc->ProcessHistStretch())
		Invalidate(TRUE);
}


// 히스토그램 처리 - 엔드인 탐색
void CColorImageProcessorView::OnHistEndin()
{
	CColorImageProcessorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDoc->ProcessHistEndin())
		Invalidate(TRUE);
}


// 히스토그램 처리 - 평활화(RGB)
void CColorImageProcessorView::OnHistEqualizeRgb()
{
	CColorImageProcessorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDoc->ProcessHistEqualizeRgb())
		Invalidate(TRUE);
}


// 히스토그램 처리 - 평활화(HSI)
void CColorImageProcessorView::OnHistEqualizeHsi()
{
	CColorImageProcessorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDoc->ProcessHistEqualizeHsi())
		Invalidate(TRUE);
}


// 영역 처리 - 엠보싱
void CColorImageProcessorView::OnAreaEmboss()
{
	CColorImageProcessorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDoc->ProcessAreaEmboss())
		Invalidate(TRUE);
}


// 영역 처리 - 엠보싱(HSI)
void CColorImageProcessorView::OnAreaEmbossHsi()
{
	CColorImageProcessorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDoc->ProcessAreaEmbossHsi())
		Invalidate(TRUE);
}


// 영역 처리 - 샤프닝
void CColorImageProcessorView::OnAreaSharp()
{
	CColorImageProcessorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDoc->ProcessAreaSharp())
		Invalidate(TRUE);
}


// 영역 처리 - 블러링
void CColorImageProcessorView::OnAreaBlur()
{
	CColorImageProcessorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDoc->ProcessAreaBlur())
		Invalidate(TRUE);
}


// 영역 처리 - 경계선 검출
void CColorImageProcessorView::OnAreaEdge()
{
	CColorImageProcessorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDoc->ProcessAreaEdge())
		Invalidate(TRUE);
}


// 색상 처리 - 색조
void CColorImageProcessorView::OnColorHue()
{
	CColorImageProcessorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDoc->ProcessColorHue())
		Invalidate(TRUE);
}


// 색상 처리 - 채도
void CColorImageProcessorView::OnColorSaturation()
{
	CColorImageProcessorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDoc->ProcessColorSaturation())
		Invalidate(TRUE);
}


// 색상 처리 - 색상 선택
void CColorImageProcessorView::OnColorPicker()
{
	CColorImageProcessorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDoc->ProcessColorPicker())
		Invalidate(TRUE);
}


// 출력 이미지를 입력 이미지로 복사
void CColorImageProcessorView::OnOutToIn()
{
	CColorImageProcessorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDoc->CopyOutputToInput())
		Invalidate(TRUE);
}


// 출력 이미지를 보조 이미지로 복사
void CColorImageProcessorView::OnOutToSub()
{
	CColorImageProcessorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDoc->CopyOutputToSub())
		Invalidate(TRUE);
}



