#pragma once

#include "Math.h"
#include "Matrix4x4.h"
#include "Vector3.h"
#include "WorldTransform.h"

// 単位行列の作成
Matrix4x4 MakeIdentityMatrix();

// 代入演算子オーバーロード
Matrix4x4& operator*=(Matrix4x4& lhm, const Matrix4x4& rhm);

// 2項演算子オーバーロード
Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2);

// 座標変換（w除算あり）
Vector3 Transform(const Vector3& v, const Matrix4x4& m);

// 行列変換と転送
void SetMatrix(WorldTransform& WT);