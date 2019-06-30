#pragma once

#include "UIWindow.h"
#include "UIOptionsItem.h"

class CUI3tButton;
class CUIFrameLineWnd;
class CUITrackButton;

class CUITrackBar : public CUIWindow, public CUIOptionsItem 
{
	friend class CUITrackButton;
public:
					CUITrackBar				();
	// CUIOptionsItem
	virtual void 	SetCurrentValue			();
	virtual void 	SaveValue				();
	virtual bool 	IsChanged				();
	virtual void 	SeveBackUpValue			();
	virtual void 	Undo					();
	virtual void	Draw					();
	virtual void	Update					();
	virtual bool	OnMouse					(float x, float y, EUIMessages mouse_action);
	virtual	void 	OnMessage				(LPCSTR message);
	// CUIWindow
			void	InitTrackBar			(Fvector2 pos, Fvector2 size);
	virtual void	Enable					(bool status);
			void	SetInvert				(bool v){m_b_invert=v;}
			bool	GetInvert				() const	{return m_b_invert;};
			void	SetStep					(float step);
			void	SetType					(bool b_float){m_b_is_float=b_float;};
			bool	GetCheck				();
			void	SetCheck				(bool b);
			int		GetIValue				(){return m_i_val;}
			float		GetFValue				(){return m_f_val;}
protected:
			void 	UpdatePos				();
			void 	UpdatePosRelativeToMouse();

    CUI3tButton*		m_pSlider;
	CUIFrameLineWnd*	m_pFrameLine;
	CUIFrameLineWnd*	m_pFrameLine_d;
	bool				m_b_invert;
	bool				m_b_is_float;
	bool				m_b_mouse_capturer;

	union{
		struct{
			float				m_f_val;
			float				m_f_max;
			float				m_f_min;
			float				m_f_step;
			float				m_f_back_up;
		};
		struct{
			int					m_i_val;
			int					m_i_max;
			int					m_i_min;
			int					m_i_step;
			int					m_i_back_up;
		};
	};
};