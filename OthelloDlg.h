﻿
// OthelloDlg.h: 头文件
//

#pragma once

#define SIZE 8
// COthelloDlg 对话框
class COthelloDlg : public CDialogEx
{
// 构造
public:
	COthelloDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OTHELLO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	bool IsPlaying;
	bool NowColor;
	int ChessBoard[SIZE][SIZE];//棋盘，-1为空，0为白，1为黑
	int index;
	int GetChessBoardColor(int, int);
	void SetChessBoardColor(int, int, int);
	void CleanChessBoard();
	void EndGame();
	CPoint GetNextSameColorChessPos(int, int, int, int);
	bool CanItPlaceChessPieces(int, int, int);
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedQuit();
	afx_msg void OnClose();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedEndgame();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
