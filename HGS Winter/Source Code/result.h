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
class CScene2D;

class CResult : public CBaseMode
{
public:

	// ���U���gUI�̎��
	enum class RESULT_UI
	{
		NONE = -1,
		RESULT_SCORE,				// ���O���S
		SURVIVED_TIME,				// ��������
		KILL,						// �L��
		ANY_BUTTON,					// �G�j�[�{�^��
		RESULT_MAX,
	};

	CResult();
	~CResult();

	HRESULT Init(HWND hWnd);			//������
	void Update();						//�X�V
	void Draw();						//�`��
	void ShowDebugInfo() {};			//�f�o�b�O���\�L

	CPlayer* GetPlayer() { return nullptr; };		//�v���C���[�擾����

private:
	void					ResultUICreate();										// ���U���gUI�̐���
	void					ResultScoreCreate();									// ���U���g�X�R�A�̐���
	void					SurvivedTimeCreate();									// �v���C���[�X�R�A�̐���
	void					NumKillCreate();										// �L�����̐���

	int m_nCntResult;	//�J�E���^
	std::vector<std::shared_ptr<CScene2D>>		m_apScene2D;						// �����L���OUI 
	std::shared_ptr<CMultiNumber>				m_pResultScore;						// ���U���g�X�R�A
	std::shared_ptr<CMultiNumber>				m_pSurvivedTime;					// ��������
	std::shared_ptr<CMultiNumber>				m_pNumKill;							// �L����

};

#endif