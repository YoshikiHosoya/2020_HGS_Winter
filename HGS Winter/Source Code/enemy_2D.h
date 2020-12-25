//------------------------------------------------------------------------------
//
//2D�G�l�~�[����  [2DEnemy.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _ENEMY_2D_H_
#define _ENEMY_2D_H_
//------------------------------------------------------------------------------
//�C���N���[�h
//------------------------------------------------------------------------------
#include "main.h"
#include "enemy.h"
#include "character_2D.h"

//------------------------------------------------------------------------------
//�O���錾
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//�N���X��`
//------------------------------------------------------------------------------
class CEnemy_2D : public CCharacter_2D,public CEnemy
{
public:

	enum ENEMY_TYPE
	{
		BLUE,			//�@���ʁ@���@�v���C���[�ǔ�
		RED,			//�ԁ@�����@�ˁ@����
		PURPLE,			//���@�����@���@�v���C���[�ǔ��H
	};


	CEnemy_2D();																						//�R���X�g���N�^
	~CEnemy_2D();																						//�f�X�g���N�^

	virtual HRESULT Init();																			//������
	virtual void Update();																			//�X�V
	virtual void Draw();																			//�`��
	virtual void ShowDebugInfo();																	//�f�o�b�O���\�L
	static std::shared_ptr<CEnemy_2D> Create(D3DXVECTOR3 pos, CEnemy_2D::ENEMY_TYPE type);			//����

	void Collision() override;
	void DamageAction();																			//�_���[�W���̃��A�N�V����
	void DeathAction();																				//���S���̃��A�N�V����
	void SetState(STATE nextstate);																	//�X�e�[�g�ύX����
	void State();																					//�X�e�[�g����

	void RedEnemy_Turn();

	void SetType(ENEMY_TYPE type);																	//�^�C�v�ݒ�

	ENEMY_TYPE GetEnemyType() { return m_type; };													//�G�̃^�C�v�擾
	D3DXVECTOR3 GetEnemyPos() override { return GetPos(); };										//���W�擾

	void MoveAI();																					//�ړ�����

	static void SetPlayerPosPtr(D3DXVECTOR3 *pPos) { m_pPlayerPos = pPos; };						//�v���C���[�̍��W�̃|�C���^�ݒ�
	static int GetEnemyNum() { return m_nNumEnemy; };
	static int GetEnemyKillNum() { return m_nKillEnemyNum; };

	static void ResetEnemyInfo();	//���Z�b�g

protected:

private:
	bool m_bMove;								//�ړ����Ă邩�ǂ���
	ENEMY_TYPE m_type;							//�^�C�v
	D3DXVECTOR3 m_DifPos;						//���W�̍���
	DIRECTION m_direction;						//����
	int m_nCntDirection;						//�����ύX
	static D3DXVECTOR3 *m_pPlayerPos;			//�v���C���[�̍��W
	static int m_nNumEnemy;						//�G�̑���
	static int m_nKillEnemyNum;					//�L�����ꂽ�G
};
#endif