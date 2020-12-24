//------------------------------------------------------------------------------
//
//�^�C�g������  [title.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _TITLE_H_
#define _TITLE_H_
//------------------------------------------------------------------------------
//�C���N���[�h
//------------------------------------------------------------------------------
#include "main.h"
#include "basemode.h"

//------------------------------------------------------------------------------
//�}�N��
//------------------------------------------------------------------------------
#define MAX_ENEMY				(3)			// �o��������Ă��̐�

//------------------------------------------------------------------------------
//�N���X��`
//------------------------------------------------------------------------------
class CScene2D;

class CTitle : public CBaseMode
{
public:
	enum class TITLE_UI
	{
		NONE = -1,
		TITLE_BLACK,			// ��
		TITLE_NAME,				// ���O���S
		TITLE_ENTER,			// Enter
		TITLE_MAX,
	};
	//���
	enum STATE
	{
		STATE_NONE = -1,				//����
		STATE_START,					//�ŏ��̉��o
		STATE_NORMAL,					//�ʏ���
		STATE_FADE,						//�t�F�[�h
	};
	CTitle();							//�R���X�g���N�^
	~CTitle();							//�f�X�g���N�^

	HRESULT Init(HWND hWnd);			//������
	void Update();						//�X�V
	void Draw();						//�`��
	void ShowDebugInfo(){};				//�f�o�b�O���\�L

	CPlayer* GetPlayer() { return nullptr; };
private:
	void TitleUICreate();				// �^�C�g��UI�̐���
	void EnemyCreate();					// �G�̐���
	void EnemyUpdate();					// �G�̍X�V

	STATE m_titlestate;					//�X�e�[�g
	int m_nCntState;					//�X�e�[�g�̃J�E���g
	D3DXVECTOR3	m_EnemyMove[MAX_ENEMY];	// �G�̈ړ���
	std::vector<std::shared_ptr<CScene2D>>		m_apScene2D;						// �^�C�g��UI
	std::vector<std::shared_ptr<CScene2D>>		m_apEnemy;							// �G

};

#endif