//------------------------------------------------------------------------------
//
//2D�v���C���[����  [3Dplayer.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _BOMB_H_
#define _BOMB_H_
//------------------------------------------------------------------------------
//�C���N���[�h
//------------------------------------------------------------------------------
#include "main.h"
#include "character_2D.h"

//------------------------------------------------------------------------------
//�O���錾
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//�N���X��`
//------------------------------------------------------------------------------
class CBomb : public CCharacter_2D
{
public:
	CBomb();																						//�R���X�g���N�^
	~CBomb();																						//�f�X�g���N�^

	virtual HRESULT Init();																			//������
	virtual void Update();																			//�X�V
	virtual void Draw();																			//�`��
	virtual void ShowDebugInfo();																	//�f�o�b�O���\�L

	void DamageAction();																			//�_���[�W���̃��A�N�V����
	void DeathAction();																				//���S���̃��A�N�V����
	void SetState(STATE nextstate);																	//�X�e�[�g�ύX����
	void Collision() override;

	void Explosion();																				//����

	static std::shared_ptr<CBomb> Create(D3DXVECTOR3 pos);											//����
	static void SetPlayerPosPtr(D3DXVECTOR3 *pPos) { m_pPlayerPos = pPos; };						//�v���C���[�̍��W�̃|�C���^�ݒ�

protected:

private:
	bool m_bMove;
	static D3DXVECTOR3 *m_pPlayerPos;			//�v���C���[�̍��W
	float m_fRotation;							//��]��

};
#endif