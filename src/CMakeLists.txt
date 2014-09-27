set(
  ITD++_SOURCES
    AIComponent.cpp AISystem.cpp AnimationComponent.cpp
    AnimationSystem.cpp Button.cpp CAnimation.cpp CApp.cpp CApp_OnCleanup.cpp
    CApp_OnEvent.cpp CApp_OnInit.cpp CApp_OnLoop.cpp CApp_OnRender.cpp
    CArea.cpp CCamera.cpp CEvent.cpp CFPS.cpp CHud.cpp CMap.cpp
    CMusic.cpp CollisionComponent.cpp ComponentFactory.cpp ControlSystem.cpp
    COptions.cpp CreditsMenu.cpp CSurface.cpp CText.cpp
    CTile.cpp DamageIndicatorComponent.cpp DOTComponent.cpp DOTSystem.cpp
    EntityFactory.cpp EntityManager.cpp Gameplay.cpp
    HealthComponent.cpp HealthSystem.cpp HighscoreMenu.cpp HudInventory.cpp
    HudTargeting.cpp InventoryComponent.cpp ItemComponent.cpp
    ItemRenderSystem.cpp ItemUsage.cpp LevelLoader.cpp MainMenu.cpp Menu.cpp
    MovementComponent.cpp MovementSystem.cpp NewGameMenu.cpp OptionsMenu.cpp
    ParticleComponent.cpp ParticleSystem.cpp ProjectileComponent.cpp
    ProjectileSystem.cpp Rectangle.cpp RendererComponent.cpp RenderSystem.cpp
    Sfx.cpp SpaceComponent.cpp SpellComponent.cpp SpellEffects.cpp
    StatsComponent.cpp TrapComponent.cpp TrapSystem.cpp TurnComponent.cpp
    TurnSystem.cpp Vector2f.cpp Visibility.cpp WeaponComponent.cpp
    WeaponSystem.cpp iml/IMLComment.cpp iml/IMLNode.cpp iml/IMLParser.cpp
    iml/IMLTag.cpp iml/IMLValue.cpp iml/utils.cpp data/dirs.cpp
)

include_directories(${SDL_INCLUDE_DIR} ${SDL_IMAGE_INCLUDE_DIRS}
  ${SDL_MIXER_INCLUDE_DIRS})

set(ALL_LIBRARIES ${SDL_LIBRARY} ${SDL_IMAGE_LIBRARIES} ${SDL_MIXER_LIBRARIES})

if(${WIN32})
  # Use of CMAKE_MFC_FLAG
  # Values:
  # 0: Use Standard Windows Libraries
  # 1: Use MFC in a Static Library
  # 2: Use MFC in a Shared DLL
  set(CMAKE_MFC_FLAG 2)
  set(RES_DIR ${ITD++_SOURCE_DIR}/share/res)
  include_directories(${RES_DIR})
  add_executable(itd++ WIN32 ${ITD++_SOURCES} ${RES_DIR}/icon.rc)
else()
  add_executable(itd++ ${ITD++_SOURCES})
endif()
target_link_libraries(itd++ ${ALL_LIBRARIES})

install(TARGETS itd++ DESTINATION ${BIN_DIR})
