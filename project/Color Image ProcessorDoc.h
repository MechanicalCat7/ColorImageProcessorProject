
// Color Image ProcessorDoc.h: CColorImageProcessorDoc 클래스의 인터페이스
//


#pragma once


class CColorImageProcessorDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CColorImageProcessorDoc() noexcept;
	DECLARE_DYNCREATE(CColorImageProcessorDoc)

// 특성입니다.
public:
	// 입력 이미지 배열
	COLORREF** inputImage = NULL;
	// 입력 이미지 높이
	int inputHeight = 0;
	// 입력 이미지 폭
	int inputWidth = 0;
	// 보조 이미지 배열
	COLORREF** subImage = NULL;
	// 보조 이미지 높이
	int subHeight = 0;
	// 보조 이미지 폭
	int subWidth = 0;
	// 출력 이미지 배열
	COLORREF** outputImage = NULL;
	// 출력 이미지 높이
	int outputHeight = 0;
	// 출력 이미지 폭
	int outputWidth = 0;

// 작업입니다.
public:
	// 보조 이미지 열기
	BOOL OpenSubfile();
	// 이차원 배열 동적 할당
	template <typename T> T** Create2DArray(int height, int width, T initValue);
	// 이차원 배열 삭제
	template <typename T> void Delete2DArray(T** target, int height);
	// 입력 이미지 삭제
	void DeleteInputImage();
	// 보조 이미지 삭제
	void DeleteSubImage();
	// 출력 이미지 삭제
	void DeleteOutputImage();
	// 입력 이미지를 그레이스케일로 변환한 이미지 생성
	COLORREF** CreateGrayscaleImage();
	// 마스크를 적용한 이미지 생성 (COLORREF, 3x3)
	COLORREF** CreateMaskedImage(double mask[][3], COLORREF** target, double addValue);
	// 마스크를 적용한 이미지 생성 (COLORREF, NxN)
	COLORREF** CreateMaskedImage(double** mask, int size, COLORREF** target, double addValue);
	// 마스크를 적용한 이미지 생성 (double, 3x3)
	double** CreateMaskedImage(double mask[][3], double** target, double addValue);

	// 값을 범위 내로 고정
	template <typename T> T Clamp(T value, T min, T max);
	// RGB 색상을 HSI 색상으로 변환 (COLORREF)
	double* RGB2HSI(COLORREF rgb);
	// RGB 색상을 HSI 색상으로 변환 (int)
	double* RGB2HSI(int r, int g, int b);
	// HSI 색상을 RGB 색상으로 변환
	COLORREF HSI2RGB(double h, double s, double i);

	// 화소점 처리 - 동일
	BOOL ProcessPointEqual();
	// 화소점 처리 - 그레이스케일
	BOOL ProcessPointGrayscale();
	// 화소점 처리 - 밝기
	BOOL ProcessPointBrightness();
	// 화소점 처리 - 반전
	BOOL ProcessPointInvert();
	// 화소점 처리 - 흑백
	BOOL ProcessPointThreshold();
	// 화소점 처리 - 감마
	BOOL ProcessPointGamma();
	// 화소점 처리 - 대비
	BOOL ProcessPointContrast();
	// 화소점 처리 - 포스터라이즈
	BOOL ProcessPointPosterize();
	// 화소점 처리 - 산술 연산
	BOOL ProcessPointMath(int opType);
	// 화소점 처리 - 논리 연산
	BOOL ProcessPointLogic(int logicType);

	// 기하학 처리 - 확대
	BOOL ProcessGeometryZoomin();
	// 확대 - 최근접 이웃
	void GeometryZoominNeareast(int scale);
	// 확대 - 양선형 보간
	void GeometryZoominBilinear(int scale);
	// 기하학 처리 - 축소
	BOOL ProcessGeometryZoomout();
	// 축소 - 최근접 이웃
	void GeometryZoomoutNeareast(int scale);
	// 축소 - 평균값
	void GeometryZoomoutAvg(int scale);
	// 기하학 처리 - 회전
	BOOL ProcessGeometryRotate();
	// 회전 - 캔버스 크기 유지
	void GeometryRotateMaintain(int angle);
	// 회전 - 캔버스 크기 확장
	void GeometryRotateExpand(int angle);
	// 기하학 처리 - 대칭
	BOOL ProcessGeometryMirror(bool isVert);
	// 기하학 처리 - 이동
	BOOL ProcessGeometryTranslate();
	// 기하학 처리 - 모핑
	BOOL ProcessGeometryBlend();

	// 히스토그램 처리 - 히스토그램 스트레칭
	BOOL ProcessHistStretch();
	// 히스토그램 처리 - 엔드인 탐색
	BOOL ProcessHistEndin();
	// 히스토그램 처리 - 평활화(RGB)
	BOOL ProcessHistEqualizeRgb();
	// 히스토그램 처리 - 평활화(HSI)
	BOOL ProcessHistEqualizeHsi();

	// 영역 처리 - 엠보싱
	BOOL ProcessAreaEmboss();
	// 영역 처리 - 엠보싱 HSI
	BOOL ProcessAreaEmbossHsi();
	// 영역 처리 - 샤프닝
	BOOL ProcessAreaSharp();
	// 영역 처리 - 블러링
	BOOL ProcessAreaBlur();
	// 영역 처리 - 경계선 검출
	BOOL ProcessAreaEdge();

	// 색상 처리 - 채도
	BOOL ProcessColorSaturation();
	// 색상 처리 - 색상 선택
	BOOL ProcessColorPicker();

	// 출력 이미지를 입력 이미지로 복사
	BOOL CopyOutputToInput();
	// 출력 이미지를 보조 이미지로 복사
	BOOL CopyOutputToSub();

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void OnCloseDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~CColorImageProcessorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	// 색상 처리 - 색조
	BOOL ProcessColorHue();
};
