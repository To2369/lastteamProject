#include"Player.h"
#include"camera.h"
#include"GamePad.h"
#include"Input/gamepad.h"
#include"StageManager.h"
#include"objectManajer.h"
#include"variable_management_class_for_hit_test.h"
#include"Graphics/DebugRenderer.h"
#include "Graphics/graphics.h"
#include"UIManajer.h"
#include"scene_management.h"
/// <summary>
/// 
/// </summary>
/// <param name="sphere">objのsphere</param>
/// <param name="Position">playerの位置</param>
/// <param name="radius">playerの半径</param>
/// <param name="outsphere">結果保存用</param>
/// <returns></returns>
bool QuadPlacement_vs_PlayerSphere(const Object::SphereQuadPlacement& sphere, const DirectX::XMFLOAT3& Position, float radius, Object::ResultSphereQuadPlacement& outsphere)
{
    using namespace DirectX;

    XMVECTOR ThisPos{ XMLoadFloat3(&Position) };
    const int SphereCount = sphere.size;
    Object::SphereQuadPlacement param{};
    for (int i = 0; i < SphereCount; i++)
    {
        XMVECTOR QuadSpherePos{ XMLoadFloat3(&sphere.pos[i].Spherepos) };
        XMVECTOR Vec = XMVector3LengthSq(XMVectorSubtract(QuadSpherePos, ThisPos));

        float dist = XMVectorGetX(Vec);
        float rad = radius + sphere.SphereRadius;
        if (dist < rad * rad)
        {
            XMStoreFloat3(&outsphere.Spherepos, QuadSpherePos);
            outsphere.type = sphere.pos[i].Type;
            return true;
        }
    }
    return false;
}

//コンストラクタ
Player::Player(ID3D11Device* device)
{
    model = std::make_unique<Model>(device, filename, true);
    Smodel = std::make_unique<Model>(device, Sfilename, Sfilename, true);
    const float scale_fcator = 0.01f;	//モデルが大きいのでスケール調整
    const float Sscale_fcator = 0.005f;
    scale = { scale_fcator, scale_fcator, scale_fcator };
    Sscale = { Sscale_fcator,Sscale_fcator,Sscale_fcator };
    color = {0,0,0,0};
    isHand = false;
    radius = 0.1f;
    Sposition = { 0,1000,0 };
    resetPosition = position;

    Graphics& graphics = Graphics::Instance();
    UIManager& ince_ui = UIManager::incetance();
    //ui作成
    {
        
        vector<unique_ptr<UI>>uis;
        ince_ui.CreateUI(graphics.GetDevice(), failepath::UI_Player_Property + L"karu.png", ObjType::cution, "cution", uis, { 1200.f,600.f});
        ince_ui.CreateUI(graphics.GetDevice(), failepath::UI_Player_Property + L"chookaru.png", ObjType::Super_cution, "Super_cution", uis, { 1200.f,600.f });
        ince_ui.CreateUI(graphics.GetDevice(), failepath::UI_Player_Property + L"juu.png", ObjType::heavy, "heavy", uis, { 1200.f,600.f });
        ince_ui.CreateUI(graphics.GetDevice(), failepath::UI_Player_Property + L"choojuu.png", ObjType::Super_heavy, "Super_heavy", uis, { 1200.f,600.f });
        ince_ui.CreateUI(graphics.GetDevice(), failepath::UI_Player_Property + L"nai.png", ObjType::null, "null", uis, { 1200.f,600.f});
        ince_ui.UiVector_Pointer_move(move(uis));
        ince_ui.CreateCanbas(UI_StringID::CanbasID::Player);
    }
    
}

//デストラクタ
Player::~Player()
{

}

//更新処理
void Player::update(float elapsedTime)
{
    elapsedTime_ = elapsedTime;

    //移動入力処理
    inputMove(elapsedTime);

    //ジャンプ入力処理
    inputJump();

    //速度処理更新
    updateVelocity(elapsedTime);

    //プレイヤーとギミックの当たり判定
    CollisionPlayerVsGimics(elapsedTime);

    //抽出注入
    ExtractionAttribute(elapsedTime);

    //注射器のアニメーション
    pullpushAnime(elapsedTime);

    ////ワールド行列の更新
    UpdateTransform();

    gamepad& gamePad = gamepad::Instance();
    if (position.y<-20)
    {
        velocity.x = 0;
        velocity.y = 0;
        velocity.z = 0;
        direction.x = 0;
        direction.z = 0;
        position.x = resetPosition.x;
        position.y = resetPosition.y+5;
        position.z = resetPosition.z;
    }
}

//描画処理
void Player::render(RenderContext* rc)
{
    DebugRenderer& debugRenderer = DebugRenderer::incetance(Graphics::Instance().GetDevice());
    Graphics& graphics = Graphics::Instance();
    if(isHand)  model->render(Graphics::Instance().GetDeviceContext(), transform, 0, color);
  
    Smodel->render(Graphics::Instance().GetDeviceContext(), Stransform, elapsedTime_, { Scolor });
    




    //衝突判定用のデバッグ球を描画
    debugRenderer.DrawSphere(position, radius, { 1,0,0,1 });

    graphics.GetDeviceContext()->OMSetBlendState(graphics.GetBlendState(2), nullptr, 0xFFFFFFFF);
}
//注射器の位置設定
void Player::updateSyringepos()
{
    XMFLOAT3 cameraeye{ Camera::instance().GetEye() };
    XMFLOAT3 camerafront{ Camera::instance().GetFront() };

    if (CubeHitFlag)
    {
        cameraeye.z += camerafront.z * 0.2f;
        cameraeye.x += camerafront.x * 0.2f;
        cameraeye.y = position.y + 0.05f;
        Sposition = cameraeye;
    }
    else if (SphereHitFlag)
    {
        cameraeye.z += camerafront.z * 0.3f;
        cameraeye.x += camerafront.x * 0.3f;
        cameraeye.y = position.y + 0.05f;
        Sposition = cameraeye;
    }
}
//操作移動
void Player::inputMove(float elapsedTime)
{
    //進行ベクトルを取得
    DirectX::XMFLOAT3 moveVec = getMoveVec();

    Move(moveVec.x, moveVec.z, this->moveSpeed);

    //進行ベクトルがゼロベクトルでない場合は入力された
    //moveVec.x != 0.0f || moveVec.y != 0.0f || moveVec.z != 0.0f;
}

//入力値から移動ベクトルを取得
DirectX::XMFLOAT3 Player::getMoveVec() const
{
    //入力情報を取得
    gamepad& gamePad = gamepad::Instance();
    float ax = gamePad.thumb_state_lx();//->getAxisLX();
    float ay = gamePad.thumb_state_ly();//->getAxisLY();

    //カメラ方向を取得
    Camera& camera = Camera::instance();
    const DirectX::XMFLOAT3& cameraFront = camera.GetFront();//camera->getFront();
    const DirectX::XMFLOAT3& cameraRight = camera.GetRight();


    //カメラ右方向ベクトルをXZ単位ベクトルに変換
    float cameraRightX = cameraRight.x;
    float cameraRightZ = cameraRight.z;
    float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
    if (cameraRightLength > 0.0f)
    {
        cameraRightX = cameraRightX / cameraRightLength;
        cameraRightZ = cameraRightZ / cameraRightLength;
    }

    //カメラ前方向ベクトルをXZ単位ベクトルに変換
    float cameraFrontX = cameraFront.x;
    float cameraFrontZ = cameraFront.z;

    //カメラ前方向ベクトルを単位ベクトル化
    float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
    if (cameraFrontLength < 0.0f)
    {
        cameraFrontX /= cameraFrontLength;
        cameraFrontZ /= cameraFrontLength;
    }

    //垂直入力値をカメラ前方向に反映し、水平方向をカメラ右方向に反映し進行ベクトルを計算する
    DirectX::XMFLOAT3 vec;
    vec.x = (cameraRightX * ax) + (cameraFrontX * ay);
    vec.z = (cameraRightZ * ax) + (cameraFrontZ * ay);

    //Y軸方向には移動しない
    vec.y = 0.0f;

    return vec;
}

//ジャンプ入力処理
void Player::inputJump()
{
    gamepad& gamePad = gamepad::Instance();
    if (gamePad.button_state(gamepad::button::a))//(スペースキー,Aボタン)
    {
        //ジャンプ回数制限
        if (jumpCount < jumpLimit)
        {
            //ジャンプ
            jumpCount++;
            jump(jumpSpeed);
        }
    }
}

void Player::OnLanding()
{
    jumpCount = 0;
}

//プレイヤーとギミックの当たり判定
void Player::CollisionPlayerVsGimics(float elapsedTime)
{
    gamepad& gamePad = gamepad::Instance();
    Objectmanajer& objMgr = Objectmanajer::incetance();
    gamepad& ince_pad = gamepad::Instance();

    //全てのギミックと総当たりで衝突処理
    int objCount = objMgr.Get_GameObjCount();
    for (int i = 0; i < objCount; i++)
    {
        Object* obj = objMgr.Get_GameObject(i);
        //衝突判定
        Object::ResultSphereQuadPlacement outsphere;

        //実際に動かす
        if (QuadPlacement_vs_PlayerSphere(obj->GetMySphere(), position, radius, outsphere))
        {
            //物を動かせるかどうか判断
            if (gamePad.button_state(gamepad::button::x) && !pushFlag)//(Q、xボタン)
            {
                pushFlag = true;
            }
            else if (gamePad.button_state(gamepad::button::y) && pushFlag)//E,Yボタン 
            {
                obj->SetVelotyXZ({ 0,0 });
                jumpCount = 0;
                color = { 0,0,0,0 };
                pushFlag = false;
                isHand = false;
            }

            if (pushFlag)
            {
                float objMovesp = velocity.x;
                switch (outsphere.type)
                {
                case Object::SphereAttribute::Right:
                {

                    objMovesp = velocity.x * elapsedTime;
                    if (!obj->GetStatic_Objflag())
                        if (!obj->GetIsWall() && !obj->GetIsObject())
                            if (obj->Get_Old_Objtype(0) == ObjType::cution || obj->Get_Old_Objtype(0) == ObjType::Super_cution)
                                if (objMovesp < 0)
                                {
                                    SceneManagement::instance().GetSe(static_cast<int>(SceneManagement::SCENE_SE::SE_OBJECT_MOVE))->play();
                                    obj->SetVelotyXZ({ objMovesp,0 });
                                }
                                else
                                {
                                    obj->SetVelotyXZ({ 0,0 });
                                }
                    jumpCount = 1;
                    position.x = outsphere.Spherepos.x;
                    position.z = outsphere.Spherepos.z;
                    color = { 1,1,1,1 };
                    angle.y = DirectX::XMConvertToRadians(270);
                    isHand = true;
                }
                break;
                case Object::SphereAttribute::Left:
                {
                    objMovesp = velocity.x * elapsedTime;
                    if (!obj->GetStatic_Objflag())
                        if (!obj->GetIsWall() && !obj->GetIsObject())
                            if (obj->Get_Old_Objtype(0) == ObjType::cution || obj->Get_Old_Objtype(0) == ObjType::Super_cution)
                                if (objMovesp > 0)
                                {
                                    SceneManagement::instance().GetSe(static_cast<int>(SceneManagement::SCENE_SE::SE_OBJECT_MOVE))->play();
                                    obj->SetVelotyXZ({ objMovesp,0 });
                                }
                                else
                                {
                                    obj->SetVelotyXZ({ 0,0 });
                                }
                    jumpCount = 1;
                    position.x = outsphere.Spherepos.x;
                    position.z = outsphere.Spherepos.z;
                    angle.y = DirectX::XMConvertToRadians(90);
                    color = { 1,1,1,1 };
                    isHand = true;
                }
                break;
                case Object::SphereAttribute::Front:

                    objMovesp = velocity.z * elapsedTime;
                    if (!obj->GetStatic_Objflag())
                        if (!obj->GetIsWall() && !obj->GetIsObject())
                            if (obj->Get_Old_Objtype(0) == ObjType::cution || obj->Get_Old_Objtype(0) == ObjType::Super_cution)
                                if (objMovesp < 0)
                                {
                                    SceneManagement::instance().GetSe(static_cast<int>(SceneManagement::SCENE_SE::SE_OBJECT_MOVE))->play();
                                    obj->SetVelotyXZ({ 0,objMovesp });
                                }
                                else
                                {
                                    obj->SetVelotyXZ({ 0,0 });
                                }
                    jumpCount = 1;
                    position.x = outsphere.Spherepos.x;
                    position.z = outsphere.Spherepos.z;
                    angle.y = DirectX::XMConvertToRadians(180);
                    color = { 1,1,1,1 };
                    isHand = true;
                    break;
                case Object::SphereAttribute::Backfront:

                    objMovesp = velocity.z * elapsedTime;
                    if (!obj->GetStatic_Objflag())
                        if (!obj->GetIsWall() && !obj->GetIsObject())
                            if (obj->Get_Old_Objtype(0) == ObjType::cution || obj->Get_Old_Objtype(0) == ObjType::Super_cution)
                                if (objMovesp > 0)
                                {
                                    SceneManagement::instance().GetSe(static_cast<int>(SceneManagement::SCENE_SE::SE_OBJECT_MOVE))->play();
                                    obj->SetVelotyXZ({ 0,objMovesp });
                                }
                                else
                                {
                                    obj->SetVelotyXZ({ 0,0 });
                                }
                    jumpCount = 1;
                    position.x = outsphere.Spherepos.x;
                    position.z = outsphere.Spherepos.z;
                    color = { 1,1,1,1 };
                    angle.y = DirectX::XMConvertToRadians(0);
                    isHand = true;
                    break;
                case Object::SphereAttribute::null:
                    break;
                }
            }
        }
    }
}

//抽出注入
void Player::ExtractionAttribute(float elapsedTime)
{
    gamepad& gamePad = gamepad::Instance();
    SceneManagement& SceneMgr = SceneManagement::instance();
    VMCFHT& ince_ray = VMCFHT::instance();
    HitResult hit;
    float mx = velocity.x * elapsedTime;
    float mz = velocity.z * elapsedTime;
   
   
    XMFLOAT3 start{ position.x,position.y + 0.1f,position.z };
    XMFLOAT3 end{ position.x + mx,position.y + 0.1f,position.z + mz };
    DirectX::XMFLOAT3 outpos = { 0,0,0 };
    Ray_ObjType type2 = Ray_ObjType::DaynamicObjects;
    Objectmanajer& objMgr = Objectmanajer::incetance();
    int objCount = objMgr.Get_GameObjCount();
    for (int i = 0; i < objCount; i++)
    {
        //四角と
        Object* obj = objMgr.Get_GameObject(i);
        if (ince_ray.RayCast(start, end, hit,*obj))
        {
            //抽出(左クリック、RBボタン)
            if (gamePad.button_state(gamepad::button::left_shoulder, trigger_mode::falling_edge) == true)
            {
                SceneMgr.GetSe(static_cast<int>(SceneManagement::SCENE_SE::SE_SYRINGE_INTERCALATE))->play();
                SceneMgr.GetSe(static_cast<int>(SceneManagement::SCENE_SE::SE_EXTRACTION))->play();
                pushType = false;
                CubeHitFlag = true;
                updateSyringepos();
                playerType = obj->Get_Original_Objtype(0);
                pullType = true;
                break;
            }
            //注入(右クリック、トリガーボタン)
            else if (gamePad.button_state(gamepad::button::right_shoulder, trigger_mode::falling_edge) && pullType)
            {
                SceneMgr.GetSe(static_cast<int>(SceneManagement::SCENE_SE::SE_SYRINGE_INTERCALATE))->play();
                SceneMgr.GetSe(static_cast<int>(SceneManagement::SCENE_SE::SE_INJECTION_SOUND))->play();
                pushType = true;
                CubeHitFlag = true;
                updateSyringepos();
                obj->Set_attribute(playerType, 0);
                pullType = false;
                break;
            }
        }
        //円柱と
        else if (objMgr.Sphere_VS_Player(position, radius, obj->GetPosition(), obj->GetRadius(), outpos))
        {
            //抽出(左クリック、RBボタン)
            if (gamePad.button_state(gamepad::button::left_shoulder, trigger_mode::falling_edge))
            {
                SceneMgr.GetSe(static_cast<int>(SceneManagement::SCENE_SE::SE_SYRINGE_INTERCALATE))->play();
                SceneMgr.GetSe(static_cast<int>(SceneManagement::SCENE_SE::SE_EXTRACTION))->play();
                pushType = false;
                SphereHitFlag = true;
                updateSyringepos();
                playerType = obj->Get_Original_Objtype(0);
                pullType = true;
                break;
            }
            //注入(右クリック、トリガーボタン)
            else if (gamePad.button_state(gamepad::button::right_shoulder, trigger_mode::falling_edge) && pullType)
            {
                SceneMgr.GetSe(static_cast<int>(SceneManagement::SCENE_SE::SE_SYRINGE_INTERCALATE))->play();
                SceneMgr.GetSe(static_cast<int>(SceneManagement::SCENE_SE::SE_INJECTION_SOUND))->play();
                SphereHitFlag = true;
                updateSyringepos();
                obj->Set_attribute(playerType, 0);
                pushType = true;
                pullType = false;
                break;
            }
        }

    }
}
//注射器のアニメーション
void Player::pullpushAnime(float elapsedTime)
{
    SceneManagement& SceneMgr = SceneManagement::instance();
    if (pullType)
    {
        Scolor = { 1,1,1,1 };
        Smodel->kefreame = Smodel->getKeyFreame(elapsedTime, Sanime::pull);
        Smodel->update_animation(*Smodel->kefreame);
        if (Smodel->animation_End)
        {
            SceneMgr.GetSe(static_cast<int>(SceneManagement::SCENE_SE::SE_SYRINGE_INTERCALATE))->stop();
            SceneMgr.GetSe(static_cast<int>(SceneManagement::SCENE_SE::SE_EXTRACTION))->stop();
            Sposition = { 5000,5000,5000 };
            CubeHitFlag = false;
            SphereHitFlag = false;
            Smodel->stop_animation = false;
            Smodel->animation_End = false;
        }
    }
    else if (pushType)
    {
        Scolor = { 1,1,1,1 };
        Smodel->kefreame = Smodel->getKeyFreame(elapsedTime, Sanime::push);
        Smodel->update_animation(*Smodel->kefreame);
        if (Smodel->animation_End)
        {
            SceneMgr.GetSe(static_cast<int>(SceneManagement::SCENE_SE::SE_SYRINGE_INTERCALATE))->stop();
            SceneMgr.GetSe(static_cast<int>(SceneManagement::SCENE_SE::SE_INJECTION_SOUND))->stop();
            playerType = Obj_attribute::null;
            Sposition = { 5000,5000,5000 };
            CubeHitFlag = false;
            SphereHitFlag = false;
            Smodel->stop_animation = false;
            Smodel->animation_End = false;
        }
    }
    else
    {
        Smodel->kefreame = Smodel->getKeyFreame(elapsedTime, Sanime::wait);
        Smodel->update_animation(*Smodel->kefreame);
        Smodel->animation_End = false;
        Smodel->stop_animation = false;
        Sposition = { 5000,5000,5000 };
    }
}