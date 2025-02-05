# MovePawn
## 목표
-  입력에 따라 움직이는 Pawn클래스를 제작해 볼것
-  Simulate Physics = false로 설정해 제작해볼것
-  드론같이 단순한 전진보다 현재 회전 상태에 따른 전진을 적용해볼것
-  중력을 적용해보고 에어 컨트롤도 구현해볼것

## Pawn클래스
구현 사항
-  이동속도, 최대 이동속도 회전속도, 회전 최대 최소 각 중력크기, 점프 속도(힘), 공중에서의 이동속도 감소를 UPROPERTY로 에디터에서 수정할수 있게 설정하였습니다.
-  충돌 컴포넌트는 Capsule, 메쉬는 Skeletal, 정면방향을 나타내기위해 Arrow Component를 사용했습니다.
-  점프는 점프중에 반복 실행되면 문제가 생김으로 bIsJump를 통해 설정했습니다.
-  전방, 옆, 위 아래를 SeepSingleByChannel를 통해 감지하고 감지한 방향의 속도를 0으로 만들어 멈추게 만들었습니다. <- (문제)
-  Jump와 이동에있어 완벽한 0보다는 오차범위를 만들어 멈추도록 작동했습니다.

문제 사항
-  ~~한번 SweepSingleByChannel에 발동될떄 움직임을 멈추는 현상~~ <- 앞에 Epsilon의 범위로 측정하고있었는데 비교전에 Epsilon보다 큰 Speed로 움직여 Epsilon보다 크게 들어가게 되어 박혀있었다.
-  AddActorLocalOffSet의 충돌을 감지하기위해 처음에 ECR_Block를 사용하였으나 이것또한 시스템에서 제공하는 방식으로 멈추기 떄문에 제외하였습니다.
-  따라서 충돌을 감지하는 방식으로 GetOverlappingComponents().Num()으로 작동하려 했으나 과제에 속도, 가속도를 활용하는 방식에 SweepSingleByChannel이 적합하다 생각하여 변환

## Drown
구현 사항
-  회전에 있어 Pitch와 Roll은 일정 범위를 넘어가면 뒤집어지기때문에 제한하였습니다
문제 사항
-  Pawn클래스와 큰 차이점은 없으나 물리충돌을 구현하지못함

추가적으로 ETriggerEvent::Triggered는 한번만 실행해야하나 Move에 있어서 반복적으로 발생하는 이유는 IA의 설정에 Trigger를 설정하지않아 IA의 입력이 여러번 되는것으로 추측한다. 따라서 명시적으로도 반복 실행을 하기위해서는 Ongoing을 쓰는것이 좋아보인다
