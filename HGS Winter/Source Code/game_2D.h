//------------------------------------------------------------------------------
//
//�Q�[������  [game.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _GAME_2D_H_
#define _GAME_2D_H_
//------------------------------------------------------------------------------
//�C���N���[�h
//------------------------------------------------------------------------------
#include "game.h"
//------------------------------------------------------------------------------
//�}�N��
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//�N���X��`
//------------------------------------------------------------------------------
class CWay;
class CScene2D;
class CMultiNumber;

class CGame_2D : public CGame
{
public:
	// �Q�[��UI�̎��
	enum class GAME_UI
	{
		NONE = -1,
		SCORE,				// �X�R�A
		TIME,				// �^�C��
		CROSS,				// �o�c ( ������ )
		HIGHSCORE,			// �n�C�X�R�A
		GAME_UI_MAX,
	};

	CGame_2D();
	~CGame_2D();
	HRESULT Init(HWND hWnd);								//������
	void Update();											//�X�V
	void Draw();											//�`��
	void ShowDebugInfo();									//�f�o�b�O���\�L

	int GetSpeed() { return m_nSpeed; };
	static int GetTime() { return m_nCntTime; };					// �������Ԃ̎擾
	DIRECTION GetDirection() { return m_direction; };
	void GameEnd();
	void SetGamestate(CGame::STATE state);
	void EnemySpawn();													//�G�o��
	void CreateEnemyGroup(D3DXVECTOR3 posOrigin);						//�G�̏W�c����
	void AddScoreMag() { m_nScoreMag++; };								//�X�R�A�{��

protected:
private:
	void					GameUICreate();							// �Q�[��UI�̐���
	void					AddTimer();								// �^�C�}�[
	void					HighScoreCreate();						// �n�C�X�R�A�̐���
	void					HighScoreUpdate();						// �n�C�X�R�A�̍X�V

	int m_nCnt;
	int m_nSpeed;										//�ړ����x
	int m_nScoreDistance;								//��������
	DIRECTION m_direction;								//����

	float m_fNextBendingPoint;							//���̋Ȃ���p�܂ł̋���
	D3DXVECTOR3 *m_pNextBendingWayPos;					//���̋Ȃ���ӏ��̍��W
	DIRECTION m_NextBendingDirection;					//���̋Ȃ���p�̕���
	bool m_bBendingFlag;								//�Ȃ���̃t���O
	int m_nBendingTime;									//�Ȃ����Ă鎞��
	int m_nTime;										//�^�C��
	static int m_nCntTime;								// �J�E���g�^�C��
	int m_nBendingCountDown;							//�Ȃ���Ƃ��̃J�E���g�_�E��

	std::shared_ptr<CMultiNumber>				m_pScoreNumber;					// �X�R�A
	std::shared_ptr<CMultiNumber>				m_pTimeNumber;					// �^�C��
	std::shared_ptr<CMultiNumber>				m_pMagnification;				// �{��
	std::shared_ptr<CMultiNumber>				m_pHighScoreNumber;				// �n�C�X�R�A

	std::vector<std::shared_ptr<CScene2D>>		m_apScene2D;					// �Q�[��UI 
	int m_nScoreMag;
};

#endif