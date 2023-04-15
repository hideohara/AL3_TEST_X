#include "MathUtilityForText.h"

Matrix4x4 MakeIdentityMatrix() {
	static const Matrix4x4 result{1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	                              0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};

	return result;
}

Matrix4x4& operator*=(Matrix4x4& lhm, const Matrix4x4& rhm) {
	Matrix4x4 result{};

	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < 4; j++) {
			for (size_t k = 0; k < 4; k++) {
				result.m[i][j] += lhm.m[i][k] * rhm.m[k][j];
			}
		}
	}
	lhm = result;
	return lhm;
}

Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result = m1;

	return result *= m2;
}

Vector3 Transform(const Vector3& v, const Matrix4x4& m) {
	float w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];

	Vector3 result{
	    (v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0]) / w,
	    (v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1]) / w,
	    (v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2]) / w};

	return result;
}

// 行列変換と転送
void SetMatrix(WorldTransform& WT) {

	//スケーリング倍率を行列に設定する
	Matrix4x4 matScale = MakeIdentityMatrix();
	matScale.m[0][0] = WT.scale_.x;
	matScale.m[1][1] = WT.scale_.y;
	matScale.m[2][2] = WT.scale_.z;

	// Z軸回転行列の各要素を設定する
	Matrix4x4 matRotZ = MakeIdentityMatrix();
	matRotZ.m[0][0] = cosf(WT.rotation_.z);
	matRotZ.m[0][1] = sinf(WT.rotation_.z);
	matRotZ.m[1][0] = -sinf(WT.rotation_.z);
	matRotZ.m[1][1] = cosf(WT.rotation_.z);

	// X軸回転行列の各要素を設定する
	Matrix4x4 matRotX = MakeIdentityMatrix();
	matRotX.m[1][1] = cosf(WT.rotation_.x);
	matRotX.m[1][2] = sinf(WT.rotation_.x);
	matRotX.m[2][1] = -sinf(WT.rotation_.x);
	matRotX.m[2][2] = cosf(WT.rotation_.x);

	// Y軸回転行列の各要素を設定する
	Matrix4x4 matRotY = MakeIdentityMatrix();
	matRotY.m[0][0] = cosf(WT.rotation_.y);
	matRotY.m[2][0] = sinf(WT.rotation_.y);
	matRotY.m[0][2] = -sinf(WT.rotation_.y);
	matRotY.m[2][2] = cosf(WT.rotation_.y);

	// XYZの各行列を掛けて回転行列を設定する
	Matrix4x4 matRot = matRotZ * matRotX * matRotY;

	// 移動量を行列に設定する
	Matrix4x4 matTrans = MakeIdentityMatrix();
	matTrans.m[3][0] = WT.translation_.x;
	matTrans.m[3][1] = WT.translation_.y;
	matTrans.m[3][2] = WT.translation_.z;

	// 単位行列を代入する
	// WT.matWorld_ = MakeIdentityMatrix();
	// matScale を掛け算して代入する
	// WT.matWorld_ *= matScale;
	// 拡大、回転、移動の各行列を掛けて変換行列を設定する
	WT.matWorld_ = matScale * matRot * matTrans;
	// WT.matWorld_ = matScale * matTrans;

	// 行列を定数バッファに転送
	WT.TransferMatrix();
}