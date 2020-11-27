#pragma once
using namespace DirectX;

class Camera 
{
public:
	Camera();
	~Camera();

public:
	//���� ��ǥ ī�޶� ��ġ�� Get,Set
	XMVECTOR GetPositionXM() const { return XMLoadFloat3(&mPosition); }
	XMFLOAT3 GetPosition() const { return mPosition; }
	void SetPosition(float x, float y, float z);
	void SetPosition(const XMFLOAT3& v);
	//ī�޶� �������͵��� �����ش� .
	XMVECTOR GetRightXM()const { return XMLoadFloat3(&mRight); }
	XMFLOAT3 GetRight()const { return mRight; }
	XMVECTOR GetUpXM() const { return XMLoadFloat3(&mUp); }
	XMFLOAT3 GetUp()const { return mUp; }
	XMVECTOR GetLookXM()const { return XMLoadFloat3(&mLook); }
	XMFLOAT3 GetLook()const { return mLook; }

	//����ü �Ӽ����� �����ش�.
	float GetNearZ() const { return mNearZ; }
	float GetFarZ() const { return mFarZ; }
	float GetAspect() const { return mAspect; }
	float GetFovY() const { return mFovY; }
	float GetFovX() const;

	// �þ� ���� ������ �����,�� ��� ũ�⸦ �����ش�.
	float GetNearWindowWidth() const;
	float GetNearWindowHeight() const;
	float GetFarWindowWidth() const;
	float GetFarWindowHeight() const;

	//����ü�� �����Ѵ� .
	void SetLens(float fovY, float aspect, float zn, float zf);

	//ī�޶���ġ�� �ü� �� ���ʹ������� ī�޶� ������ �����Ѵ� .
	void LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp);
	void LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up);
	//�þ� ��İ� ���� ����� �����ش�.
	XMMATRIX View()const { return XMLoadFloat4x4(&mView); }
	XMMATRIX Proj()const { return XMLoadFloat4x4(&mProj); }
	XMMATRIX ViewProj()const { return View() * Proj(); }
	//ī�޶� �Ÿ� d�� ŭ Ⱦ�̵�, ���̵��Ѵ� .
	void Strafe(float d);
	void Walk(float d);
	//ī�޶� ȸ���Ѵ�.
	void Pitch(float angle);
	void RotateY(float angle);
	//�������Ӹ���, ī�޶� ��ġ�� ������ ���� �� ��
	//�� �޼��带 ȣ���ؼ� �þ������ �籸���Ѵ� .
	void UpdateViewMatrix();
private:
	//�� �� �� �� �� �� �� ī �� �� �� ǥ ��
	XMFLOAT3 mPosition; //�þ߰�������
	XMFLOAT3 mRight;//�þ߰���x��
	XMFLOAT3 mUp; //�þ߰���y��
	XMFLOAT3 mLook; //�þ߰���z��
	//����ü�Ӽ���
	float mNearZ;
	float mFarZ;
	float mAspect;
	float mFovY;
	float mNearWindowHeight;
	float mFarWindowHeight;
	//�þ���İ� �������
	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProj;

};