
// Color Image ProcessorView.h: CColorImageProcessorView 클래스의 인터페이스
//

#pragma once


class CColorImageProcessorView : public CView
{
protected: // serialization에서만 만들어집니다.
	CColorImageProcessorView() noexcept;
	DECLARE_DYNCREATE(CColorImageProcessorView)

// 특성입니다.
public:
	CColorImageProcessorDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 구현입니다.
public:
	virtual ~CColorImageProcessorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPointEqual();
	afx_msg void OnPointGrayscale();
	afx_msg void OnPointInvert();
	afx_msg void OnPointThreshold();
	afx_msg void OnGeometryZoomin();
	afx_msg void OnGeometryZoomout();
	afx_msg void OnAreaEmboss();
	afx_msg void OnColorSaturation();
	afx_msg void OnColorPicker();
	afx_msg void OnMirrorHori();
	afx_msg void OnMirrorVert();
	afx_msg void OnGeometryTranslate();
	afx_msg void OnGeometryRotate();
	afx_msg void OnAreaSharp();
	afx_msg void OnAreaBlur();
	afx_msg void OnAreaEmbossHsi();
	afx_msg void OnPointGamma();
	afx_msg void OnPointContrast();
	afx_msg void OnHistEqualizeRgb();
	afx_msg void OnHistEqualizeHsi();
	afx_msg void OnPointPosterize();
	afx_msg void OnHistStretch();
	afx_msg void OnHistEndin();
	afx_msg void OnAreaEdge();
	afx_msg void OnPointBrightness();
	afx_msg void OnSubfileOpen();
	afx_msg void OnOutToIn();
	afx_msg void OnOutToSub();
	afx_msg void OnLogicAnd();
	afx_msg void OnLogicOr();
	afx_msg void OnLogicXor();
	afx_msg void OnMathAdd();
	afx_msg void OnMathSubtract();
	afx_msg void OnMathMultiply();
	afx_msg void OnMathDivide();
	afx_msg void OnGeometryBlend();
	afx_msg void OnColorHue();
};

#ifndef _DEBUG  // Color Image ProcessorView.cpp의 디버그 버전
inline CColorImageProcessorDoc* CColorImageProcessorView::GetDocument() const
   { return reinterpret_cast<CColorImageProcessorDoc*>(m_pDocument); }
#endif

