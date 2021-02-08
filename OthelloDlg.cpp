
// OthelloDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "Othello.h"
#include "OthelloDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COthelloDlg 对话框



COthelloDlg::COthelloDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_OTHELLO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COthelloDlg::DoDataExchange(CDataExchange* pDX)
{
	IsPlaying = false;
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			ChessBoard[i][j] = -1;
		}
	}
	//初始化棋盘
	CDialogEx::DoDataExchange(pDX);
}

int COthelloDlg::GetChessBoardColor(int nx, int ny)
{
	return ChessBoard[ny][nx];
}

void COthelloDlg::SetChessBoardColor(int nx, int ny, int color)
{
	ChessBoard[ny][nx] = color;
	CDC* dc = this->GetDC();
	CPen pen(PS_SOLID, 2, RGB(0, 0, 0));
	dc->SelectObject(pen);
	if (color == 0)//白棋
	{
		CBrush brush_w(RGB(255, 255, 255));
		const CPoint o(90 * nx + 85, 90 * ny + 95);//圆心
		dc->SelectObject(brush_w);
		dc->Ellipse(o.x - 30, o.y - 30, o.x + 30, o.y + 30);
	}
	else if (color == 1)//黑棋
	{
		CBrush brush_b(RGB(0, 0, 0));
		const CPoint o(90 * nx + 85, 90 * ny + 95);//圆心
		dc->SelectObject(brush_b);
		dc->Ellipse(o.x - 30, o.y - 30, o.x + 30, o.y + 30);
	}
}

void COthelloDlg::CleanChessBoard()
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			ChessBoard[i][j] = -1;
		}
	}
	Invalidate();
}

void COthelloDlg::EndGame()
{
	CleanChessBoard();
	IsPlaying = false;
	//index = -1;
	GetDlgItem(IDC_START)->SetWindowTextW(L"开始游戏");
	GetDlgItem(IDC_ENDGAME)->EnableWindow(FALSE);
	//GetDlgItem(IDC_REPENTANCE)->EnableWindow(FALSE);
	//GetDlgItem(IDC_SAVE)->EnableWindow(FALSE);
}

CPoint COthelloDlg::GetNextSameColorChessPos(int nx, int ny, int direction, int TestColor = 2)
{
	int x = nx, y = ny;
	int color = TestColor;
	if (color == 2)
		color = GetChessBoardColor(x, y);
	switch (direction)//注意：Windows系统和数组坐标以左上角为原点，所以上下要相反
	{
	case 0://左
		while (1)
		{
			x--;
			if (x < 0)//未找到
				return CPoint(SIZE, SIZE);
			if (GetChessBoardColor(x, y) == -1)//中间有空格
				return CPoint(SIZE, SIZE);
			if (GetChessBoardColor(x, y) == color)
				return CPoint(x, y);
		}
		//函数肯定会返回，无需break
	case 1://左上
		while (1)
		{
			x--;
			y--;
			if (x < 0 || y < 0)//未找到
				return CPoint(SIZE, SIZE);
			if (GetChessBoardColor(x, y) == -1)//中间有空格
				return CPoint(SIZE, SIZE);
			if (GetChessBoardColor(x, y) == color)
				return CPoint(x, y);
		}
	case 2://上
		while (1)
		{
			y--;
			if (y < 0)//未找到
				return CPoint(SIZE, SIZE);
			if (GetChessBoardColor(x, y) == -1)//中间有空格
				return CPoint(SIZE, SIZE);
			if (GetChessBoardColor(x, y) == color)
				return CPoint(x, y);
		}
	case 3://右上
		while (1)
		{
			x++;
			y--;
			if (x >= SIZE || y < 0)//未找到
				return CPoint(SIZE, SIZE);
			if (GetChessBoardColor(x, y) == -1)//中间有空格
				return CPoint(SIZE, SIZE);
			if (GetChessBoardColor(x, y) == color)
				return CPoint(x, y);
		}
	case 4://右
		while (1)
		{
			x++;
			if (x >= SIZE)//未找到
				return CPoint(SIZE, SIZE);
			if (GetChessBoardColor(x, y) == -1)//中间有空格
				return CPoint(SIZE, SIZE);
			if (GetChessBoardColor(x, y) == color)
				return CPoint(x, y);
		}
	case 5://右下
		while (1)
		{
			x++;
			y++;
			if (x >= SIZE || y >=SIZE)//未找到
				return CPoint(SIZE, SIZE);
			if (GetChessBoardColor(x, y) == -1)//中间有空格
				return CPoint(SIZE, SIZE);
			if (GetChessBoardColor(x, y) == color)
				return CPoint(x, y);
		}
	case 6://下
		while (1)
		{
			y++;
			if (y >=SIZE)//未找到
				return CPoint(SIZE, SIZE);
			if (GetChessBoardColor(x, y) == -1)//中间有空格
				return CPoint(SIZE, SIZE);
			if (GetChessBoardColor(x, y) == color)
				return CPoint(x, y);
		}
	case 7://左下
		while (1)
		{
			x--;
			y++;
			if (x < 0 || y >=SIZE)//未找到
				return CPoint(SIZE, SIZE);
			if (GetChessBoardColor(x, y) == -1)//中间有空格
				return CPoint(SIZE, SIZE);
			if (GetChessBoardColor(x, y) == color)
				return CPoint(x, y);
		}
	}
}

bool COthelloDlg::CanItPlaceChessPieces(int x, int y, int color)
{
	int sum = 0;
	for (int direction = 0; direction < 8; direction++)
	{
		int count;
		switch (direction)
		{
		case 2://上
		case 6://下
		{
			int pos = GetNextSameColorChessPos(x, y, direction, color).y;
			count = abs(pos - y) - 1;
			//计算y轴坐标的差
			if (pos == SIZE)//没找到
				count = 0;
		}
		break;
		default://其它方向，计算x轴坐标的差
		{
			int pos = GetNextSameColorChessPos(x, y, direction, color).x;
			count = abs(pos - x) - 1;
			//计算y轴坐标的差
			if (pos == SIZE)//没找到
				count = 0;
		}
		}
		sum += count;
	}
	return sum;//return (sum > 0);
}

BEGIN_MESSAGE_MAP(COthelloDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_QUIT, &COthelloDlg::OnBnClickedQuit)
	ON_WM_CLOSE()
	ON_WM_SETCURSOR()
	ON_BN_CLICKED(IDC_START, &COthelloDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_ENDGAME, &COthelloDlg::OnBnClickedEndgame)
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// COthelloDlg 消息处理程序

BOOL COthelloDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	SetBackgroundImage(IDB_BACKGROUNDIMAGE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void COthelloDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);
		CPen pen(PS_SOLID, 2, RGB(0, 0, 0));
		dc.SelectObject(pen);
		for (int i = 0; i < (SIZE+1)/*黑白棋下在格子里，要多画一条线*/; i++)
		{
			dc.MoveTo(40, 50 + i * 90);
			dc.LineTo(760, 50 + i * 90);
		}//绘制棋盘横线
		for (int i = 0; i < (SIZE + 1); i++)
		{
			dc.MoveTo(40 + i * 90, 50);
			dc.LineTo(40 + i * 90, 770);
		}//绘制棋盘竖线
		for (int nx = 0; nx < SIZE; nx++)
		{
			for (int ny = 0; ny < SIZE; ny++)
			{

				int color = GetChessBoardColor(nx, ny);
				if (color == 0)//白棋
				{
					CBrush brush_w(RGB(255, 255, 255));
					const CPoint o(90 * nx + 85, 90 * ny + 95);//圆心
					dc.SelectObject(brush_w);
					dc.Ellipse(o.x - 30, o.y - 30, o.x + 30, o.y + 30);
				}
				else if (color == 1)//黑棋
				{
					CBrush brush_b(RGB(0, 0, 0));
					const CPoint o(90 * nx + 85, 90 * ny + 95);//圆心
					dc.SelectObject(brush_b);
					dc.Ellipse(o.x - 30, o.y - 30, o.x + 30, o.y + 30);
				}
			}
		}
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR COthelloDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void COthelloDlg::OnBnClickedQuit()
{
	if (!IsPlaying || MessageBoxW(L"正在游戏中，确定要退出吗？", L"双人五子棋", MB_YESNO | MB_ICONQUESTION) == IDYES)
		EndDialog(0);
}


void COthelloDlg::OnClose()
{
	if (!IsPlaying || MessageBoxW(L"正在游戏中，确定要退出吗？", L"双人五子棋", MB_YESNO | MB_ICONQUESTION) == IDYES)
		CDialogEx::OnClose();
}


BOOL COthelloDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(&point);
	if (!IsPlaying || point.x < 40 || point.x>760 || point.y < 50 || point.y>770)
		return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
	if (NowColor == 1)//黑棋
		SetCursor(LoadCursorW(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDC_BLACK)));
	else
		SetCursor(LoadCursorW(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDC_WHITE)));
	return TRUE;
}


void COthelloDlg::OnBnClickedStart()
{
	if (IsPlaying && MessageBoxW(L"确定要重玩吗？", L"双人五子棋", MB_YESNO | MB_ICONQUESTION) == IDNO)
		return;
	GetDlgItem(IDC_START)->SetWindowTextW(L"重玩");
	IsPlaying = true;
	NowColor = 1;//黑先
	index = 0;
	GetDlgItem(IDC_ENDGAME)->EnableWindow(TRUE);
	//GetDlgItem(IDC_REPENTANCE)->EnableWindow(FALSE);
	//GetDlgItem(IDC_SAVE)->EnableWindow(TRUE);
	CleanChessBoard();
	SetChessBoardColor(SIZE / 2 - 1, SIZE / 2 - 1, 1);
	SetChessBoardColor(SIZE / 2, SIZE / 2, 1);
	SetChessBoardColor(SIZE / 2, SIZE / 2 - 1, 0);
	SetChessBoardColor(SIZE / 2 - 1, SIZE / 2, 0);
	//黑白棋初始有四个棋子
}


void COthelloDlg::OnBnClickedEndgame()
{
	if (MessageBoxW(L"确定要结束本局吗？", L"双人五子棋", MB_YESNO | MB_ICONQUESTION) == IDYES)
		EndGame();
}


void COthelloDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (!IsPlaying || point.x < 40 || point.x>760 || point.y < 50 || point.y>770)
		return;
	int x = int(round((point.x - 45 - 40) / 90.0));
	int y = int(round((point.y - 45 - 50) / 90.0));
	//将鼠标坐标转为数组下标
	if (GetChessBoardColor(x, y) != -1)//如果已有棋子
		return;
	if (!CanItPlaceChessPieces(x, y, NowColor))
		return;
	SetChessBoardColor(x, y, NowColor);
	for (int direction = 0; direction < 8; direction++)
	{
		CPoint pt = GetNextSameColorChessPos(x, y, direction);
		if (pt.x == SIZE)
			continue;
		switch (direction)//注意：Windows系统和数组坐标以左上角为原点，所以上下要相反
		{
		case 0://左
			for (int nx = pt.x + 1; nx < x; nx++)//注意符号
				SetChessBoardColor(nx, y, NowColor);
			break;
		case 1://左上
			for (int nx = pt.x + 1, ny = pt.y + 1; nx < x; nx++, ny++)//注意符号
				SetChessBoardColor(nx, ny, NowColor);
			break;
		case 2://上
			for (int ny = pt.y + 1; ny < y; ny++)//注意符号
				SetChessBoardColor(x, ny, NowColor);
			break;
		case 3://右上
			for (int nx = pt.x - 1, ny = pt.y + 1; nx > x; nx--, ny++)//注意符号
				SetChessBoardColor(nx, ny, NowColor);
			break;
		case 4://右
			for (int nx = pt.x - 1; nx > x; nx--)//注意符号
				SetChessBoardColor(nx, y, NowColor);
			break;
		case 5://右下
			for (int nx = pt.x - 1, ny = pt.y - 1; nx > x; nx--, ny--)//注意符号
				SetChessBoardColor(nx, ny, NowColor);
			break;
		case 6://下
			for (int ny = pt.y - 1; ny > y; ny--)//注意符号
				SetChessBoardColor(x, ny, NowColor);
			break;
		case 7://左下
			for (int nx = pt.x + 1, ny = pt.y - 1; nx < x; nx++, ny--)//注意符号
				SetChessBoardColor(nx, ny, NowColor);
			break;
		}
		
	}
	bool b = false;
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			if (CanItPlaceChessPieces(i, j, !NowColor))
			{
				NowColor = (!NowColor);
				b = true;
				break;
			}
		}
		if (b)
			break;
	}
	//黑白棋不一定是轮流下，如果一方无棋可下则另一方一直下，直到那一方可以下
	index++;
	//order[index].x = x;
	//order[index].y = y;
	//GetDlgItem(IDC_REPENTANCE)->EnableWindow(index > -1);
	//如果可以悔棋，取消禁用“悔棋”按钮，否则禁用“悔棋”按钮
	SendMessage(WM_SETCURSOR);
	//以上为放置棋子
	if (index == (SIZE * SIZE-4))
	{
		int w = 0, b = 0;
		for (int i = 0; i < SIZE; i++)
		{
			for (int j = 0; j < SIZE; j++)
			{
				if (GetChessBoardColor(i, j) == 0)
					w++;
				else
					b++;
			}
		}
		if (w > b)
			MessageBoxW(L"白棋胜利！", L"双人黑白棋", MB_OK | MB_ICONINFORMATION);
		else if (b > w)
			MessageBoxW(L"黑棋胜利！", L"双人黑白棋", MB_OK | MB_ICONINFORMATION);
		else
			MessageBoxW(L"平局！", L"双人黑白棋", MB_OK | MB_ICONINFORMATION);
	}
	//以上为判断胜负
}
