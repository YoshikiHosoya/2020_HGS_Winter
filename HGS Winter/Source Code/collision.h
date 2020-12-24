//------------------------------------------------------------------------------
//
//�����蔻�菈��  [collision.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _COLLISION_H_
#define _COLLISION_H_
//------------------------------------------------------------------------------
//�C���N���[�h
//------------------------------------------------------------------------------
#include "main.h"

//------------------------------------------------------------------------------
//�O���錾
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//�N���X��`
//------------------------------------------------------------------------------
class CCollision
{
public:
	CCollision();
	~CCollision();

	//�R���W�����̎��
	enum COLLISION_TYPE
	{
		COLLISION_CHARACTER,		//�L�����N�^�[
		COLLISION_WEAPON,			//����
		COLLISION_EXPLOSION,		//����
		COLLISION_OBJECT,			//�I�u�W�F�N�g
		COLLISION_CAMERA,			//�J����
	};
	void ShowDebugInfo();																				//�f�o�b�O���\�L
	static std::unique_ptr<CCollision> Create(D3DXMATRIX *pMtx, COLLISION_TYPE ColisionType);			//��������

private:
	D3DXMATRIX *m_pParentMtx;			//������̃}�g���b�N�X
	MODEL_VTX m_ModelVtx;				//���Ŕ�����Ƃ��Ɏg���@�ő咸�_�ƍŏ����_
	float m_fRadius;					//���a
	float m_fLength;					//����̒���

#ifdef _DEBUG
#endif // _DEBUG

};
#endif