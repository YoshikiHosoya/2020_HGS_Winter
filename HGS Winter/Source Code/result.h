//------------------------------------------------------------------------------
//
//���U���g����  [result.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _RESULT_H_
#define _RESULT_H_
//------------------------------------------------------------------------------
//�C���N���[�h
//------------------------------------------------------------------------------
#include "main.h"
#include "basemode.h"
//------------------------------------------------------------------------------
//�}�N��
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//�N���X��`
//------------------------------------------------------------------------------
class CMultiNumber;

class CResult : public CBaseMode
{
public:

	// ���U���gUI�̎��
	enum class RANKING_UI
	{
		NONE = -1,
		RESULT_SCORE,				// ���O���S
		RANKING_SCORE,				// �v���C���[�X�R�A
		LIVED_TIME,					// ��������
		KILL,						// �L��
		ANY_BUTTON,					// �G�j�[�{�^��
		RANKING_MAX,
	};

	CResult();
	~CResult();

	HRESULT Init(HWND hWnd);			//������
	void Uninit();						//�I��
	void Update();						//�X�V
	void Draw();						//�`��
	void ShowDebugInfo() {};			//�f�o�b�O���\�L

	CPlayer* GetPlayer() { return nullptr; };		//�v���C���[�擾����

private:
	int m_nCntResult;	//�J�E���^
	//std::vector<std::shared_ptr<CScene2D>>		m_apScene2D;						// �����L���OUI 

};

#endif