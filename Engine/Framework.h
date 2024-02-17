#pragma once
class Framework
{
public:
	//初期化
	virtual void Initialize();
	//終了
	virtual void Finalize();
	//毎フレーム更新
	virtual void Update();
	//描画
	virtual void Draw() = 0;
	//終了チェック
	virtual bool IsEndRequst() { return endRequst_; }
	void Run(); //実行
	virtual ~Framework() = default; //仮想フレームワーク

private:
	bool endRequst_;
};

