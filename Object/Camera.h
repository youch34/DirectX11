#pragma once
using namespace DirectX;

class Camera 
{
public:
	Camera();
	~Camera();

public:
	//월드 좌표 카메라 위치를 Get,Set
	XMVECTOR GetPositionXM() const { return XMLoadFloat3(&mPosition); }
	XMFLOAT3 GetPosition() const { return mPosition; }
	void SetPosition(float x, float y, float z);
	void SetPosition(const XMFLOAT3& v);
	//카메라 기저벡터들을 돌려준다 .
	XMVECTOR GetRightXM()const { return XMLoadFloat3(&mRight); }
	XMFLOAT3 GetRight()const { return mRight; }
	XMVECTOR GetUpXM() const { return XMLoadFloat3(&mUp); }
	XMFLOAT3 GetUp()const { return mUp; }
	XMVECTOR GetLookXM()const { return XMLoadFloat3(&mLook); }
	XMFLOAT3 GetLook()const { return mLook; }

	//절두체 속성들을 돌려준다.
	float GetNearZ() const { return mNearZ; }
	float GetFarZ() const { return mFarZ; }
	float GetAspect() const { return mAspect; }
	float GetFovY() const { return mFovY; }
	float GetFovX() const;

	// 시야 공간 기준의 가까운,먼 평면 크기를 돌려준다.
	float GetNearWindowWidth() const;
	float GetNearWindowHeight() const;
	float GetFarWindowWidth() const;
	float GetFarWindowHeight() const;

	//절두체를 설정한다 .
	void SetLens(float fovY, float aspect, float zn, float zf);

	//카메라위치와 시선 및 위쪽방향으로 카메라 공간을 설정한다 .
	void LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp);
	void LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up);
	//시야 행렬과 투영 행렬을 돌려준다.
	XMMATRIX View()const { return XMLoadFloat4x4(&mView); }
	XMMATRIX Proj()const { return XMLoadFloat4x4(&mProj); }
	XMMATRIX ViewProj()const { return View() * Proj(); }
	//카메라를 거리 d만 큼 횡이동, 축이동한다 .
	void Strafe(float d);
	void Walk(float d);
	//카메라를 회전한다.
	void Pitch(float angle);
	void RotateY(float angle);
	//매프레임마다, 카메라 위치나 방향을 수정 한 후
	//이 메서드를 호출해서 시야행렬을 재구축한다 .
	void UpdateViewMatrix();
private:
	//세 계 공 간 기 준 의 카 메 라 좌 표 계
	XMFLOAT3 mPosition; //시야공간원점
	XMFLOAT3 mRight;//시야공간x축
	XMFLOAT3 mUp; //시야공간y축
	XMFLOAT3 mLook; //시야공간z축
	//절두체속성들
	float mNearZ;
	float mFarZ;
	float mAspect;
	float mFovY;
	float mNearWindowHeight;
	float mFarWindowHeight;
	//시야행렬과 투영행렬
	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProj;

};