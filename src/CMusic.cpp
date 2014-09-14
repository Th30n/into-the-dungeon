/*
 * Into the Dungeon++
 *
 * Copyright (C) 2014 Teon Banek
 *
 * This file is part of Into the Dungeon++.
 * Into the Dungeon++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Into the Dungeon++ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Into the Dungeon++. If not, see <http://www.gnu.org/licenses/>
 *
 * Author(s): Teon Banek <intothedungeon@gmail.com>
 */
#include "CMusic.h"

CMusic CMusic::SoundControl;

CMusic::CMusic()
  : EquipChest1(0), EquipChest2(0), EquipChest(0),
    DoorOpen1(0), DoorOpen2(0), DoorOpen(0),
    EquipFeet1(0), EquipFeet2(0), EquipFeet(0),
    EquipHead1(0), EquipHead2(0), EquipHead(0),
    Walk1(0), Walk2(0), Walk(0),
    MaceHit1(0), MaceHit2(0), MaceHit(0),
    NeckSound1(0), NeckSound2(0), NeckSound(0),
    PotionSound1(0), PotionSound2(0), PotionSound(0),
    ScrollSound1(0), ScrollSound2(0), ScrollSound(0),
    SwordHit1(0), SwordHit2(0), SwordHit(0),
    LevelUp(0),
    Descend(0),
    Drink1(0), Drink2(0), Drink(0),
    Miss1(0), Miss2(0), Miss(0),
    Drop1(0), Drop2(0), Drop(0),
    ProjectileHit1(0), ProjectileHit2(0), ProjectileHit(0),
    EquipWeapon1(0), EquipWeapon2(0), EquipWeapon(0),
    EquipShield1(0), EquipShield2(0), EquipShield(0),
    EquipRing1(0), EquipRing2(0), EquipRing(0),
    EquipNeck1(0), EquipNeck2(0), EquipNeck(0),
    WeaponSound1(0), WeaponSound2(0), WeaponSound(0),
    ShieldSound1(0), ShieldSound2(0), ShieldSound(0),
    HeadSound1(0), HeadSound2(0), HeadSound(0),
    ChestSound1(0), ChestSound2(0), ChestSound(0),
    FeetSound1(0), FeetSound2(0), FeetSound(0),
    RingSound1(0), RingSound2(0), RingSound(0),
    TurnPage1(0), TurnPage2(0), TurnPage(0),
    music_on_(true), sound_on_(true)
{
}

Mix_Music *CMusic::OnLoad(const char *file)
{
  Mix_Music *music = NULL;
  if ((music = Mix_LoadMUS(file)) == NULL) {
    return NULL;
  }
  return music;
}

void CMusic::ChangeMusic(Mix_Music *music)
{
  if (music_on_ && music) {
    if (Mix_PlayingMusic()) {
      Mix_FadeOutMusic(1000);
    }
    Play(music, -1);
  }
}

bool CMusic::Play(Mix_Music *music, int loop_number)
{
  if (music == NULL) {
    return false;
  }
  if (Mix_PlayMusic(music, loop_number)) {
    return true;
  }
  return false;
}

void CMusic::PlaySFX(Mix_Chunk *sound, int loop_number)
{
  if (sound_on_ == false) {
    return;
  }
  if (sound) {
    Mix_PlayChannel(-1, sound, loop_number);
  }
  SoundVariance();
}

bool CMusic::LoadSounds()
{
  bool Return = false;
  
  if ((EquipChest1 = Mix_LoadWAV("./audio/sounds/chest_equip_1.ogg")) &&
      (EquipChest2 = Mix_LoadWAV("./audio/sounds/chest_equip_2.ogg")) &&
      (DoorOpen1 = Mix_LoadWAV("./audio/sounds/door_open_1.ogg")) &&
      (DoorOpen2 = Mix_LoadWAV("./audio/sounds/door_open_2.ogg")) &&
      (EquipFeet1 = Mix_LoadWAV("./audio/sounds/feet_equip_1.ogg")) &&
      (EquipFeet2 = Mix_LoadWAV("./audio/sounds/feet_equip_2.ogg")) &&
      (EquipHead1 = Mix_LoadWAV("./audio/sounds/head_equip_1.ogg")) &&
      (EquipHead2 = Mix_LoadWAV("./audio/sounds/head_equip_2.ogg")) &&
      (Walk1 = Mix_LoadWAV("./audio/sounds/walk_1.ogg")) &&
      (Walk2 = Mix_LoadWAV("./audio/sounds/walk_2.ogg")) &&
      (MaceHit1 = Mix_LoadWAV("./audio/sounds/mace_hit_1.ogg")) &&
      (MaceHit2 = Mix_LoadWAV("./audio/sounds/mace_hit_2.ogg")) &&
      (NeckSound1 = Mix_LoadWAV("./audio/sounds/neck_sound_1.ogg")) &&
      (NeckSound2 = Mix_LoadWAV("./audio/sounds/neck_sound_2.ogg")) &&
      (PotionSound1 = Mix_LoadWAV("./audio/sounds/potion_sound_1.ogg")) &&
      (PotionSound2 = Mix_LoadWAV("./audio/sounds/potion_sound_2.ogg")) &&
      (ScrollSound1 = Mix_LoadWAV("./audio/sounds/scroll_sound_1.ogg")) &&
      (ScrollSound2 = Mix_LoadWAV("./audio/sounds/scroll_sound_2.ogg")) &&
      (SwordHit1 = Mix_LoadWAV("./audio/sounds/sword_hit_1.ogg")) &&
      (SwordHit2 = Mix_LoadWAV("./audio/sounds/sword_hit_2.ogg")) &&
      (LevelUp = Mix_LoadWAV("./audio/sounds/level_up.ogg")) &&
      (Descend = Mix_LoadWAV("./audio/sounds/descend.ogg")) &&
      (Drink1 = Mix_LoadWAV("./audio/sounds/drink_1.ogg")) &&
      (Drink2 = Mix_LoadWAV("./audio/sounds/drink_2.ogg")) &&
      (Miss1 = Mix_LoadWAV("./audio/sounds/miss_1.ogg")) &&
      (Miss2 = Mix_LoadWAV("./audio/sounds/miss_2.ogg")) &&
      (Drop1 = Mix_LoadWAV("./audio/sounds/drop_1.ogg")) &&
      (Drop2 = Mix_LoadWAV("./audio/sounds/drop_2.ogg")) &&
      (ProjectileHit1 = Mix_LoadWAV("./audio/sounds/projectile_hit_1.ogg")) &&
      (ProjectileHit2 = Mix_LoadWAV("./audio/sounds/projectile_hit_2.ogg")) &&
      (EquipWeapon1 = Mix_LoadWAV("./audio/sounds/weapon_equip_1.ogg")) &&
      (EquipWeapon2 = Mix_LoadWAV("./audio/sounds/weapon_equip_2.ogg")) &&
      (EquipShield1 = Mix_LoadWAV("./audio/sounds/shield_equip_1.ogg")) &&
      (EquipShield2 = Mix_LoadWAV("./audio/sounds/shield_equip_2.ogg")) &&
      (EquipRing1 = Mix_LoadWAV("./audio/sounds/ring_equip_1.ogg")) &&
      (EquipRing2 = Mix_LoadWAV("./audio/sounds/ring_equip_2.ogg")) &&
      (EquipNeck1 = Mix_LoadWAV("./audio/sounds/neck_equip_1.ogg")) &&
      (EquipNeck2 = Mix_LoadWAV("./audio/sounds/neck_equip_2.ogg")) &&
      (WeaponSound1 = Mix_LoadWAV("./audio/sounds/weapon_sound_1.ogg")) &&
      (WeaponSound2 = Mix_LoadWAV("./audio/sounds/weapon_sound_2.ogg")) &&
      (ShieldSound1 = Mix_LoadWAV("./audio/sounds/shield_sound_1.ogg")) &&
      (ShieldSound2 = Mix_LoadWAV("./audio/sounds/shield_sound_2.ogg")) &&
      (HeadSound1 = Mix_LoadWAV("./audio/sounds/head_sound_1.ogg")) &&
      (HeadSound2 = Mix_LoadWAV("./audio/sounds/head_sound_2.ogg")) &&
      (ChestSound1 = Mix_LoadWAV("./audio/sounds/chest_sound_1.ogg")) &&
      (ChestSound2 = Mix_LoadWAV("./audio/sounds/chest_sound_2.ogg")) &&
      (FeetSound1 = Mix_LoadWAV("./audio/sounds/feet_sound_1.ogg")) &&
      (FeetSound2 = Mix_LoadWAV("./audio/sounds/feet_sound_2.ogg")) &&
      (RingSound1 = Mix_LoadWAV("./audio/sounds/ring_sound_1.ogg")) &&
      (RingSound2 = Mix_LoadWAV("./audio/sounds/ring_sound_2.ogg")) &&
      (TurnPage1 = Mix_LoadWAV("./audio/sounds/turn_page_1.ogg")) &&
      (TurnPage2 = Mix_LoadWAV("./audio/sounds/turn_page_2.ogg"))) {
    Return = true;
  }
  
  EquipChest = EquipChest1;
  DoorOpen = DoorOpen1;
  EquipFeet = EquipFeet1;
  EquipHead = EquipHead1;
  Walk = Walk1;
  MaceHit = MaceHit1;
  NeckSound = NeckSound1;
  PotionSound = PotionSound1;
  ScrollSound = ScrollSound1;
  SwordHit = SwordHit1;
  Drink = Drink1;
  Miss = Miss1;
  Drop = Drop1;
  ProjectileHit = ProjectileHit1;
  EquipWeapon = EquipWeapon1;
  EquipShield = EquipShield1;
  EquipRing = EquipRing1;
  EquipNeck = EquipNeck1;
  WeaponSound = WeaponSound1;
  ShieldSound = ShieldSound1;
  HeadSound = HeadSound1;
  ChestSound = ChestSound1;
  FeetSound = FeetSound1;
  RingSound = RingSound1;
  TurnPage = TurnPage1;
  
  return Return;
}

void CMusic::SoundVariance()
{
  if (EquipChest == EquipChest1) {
    EquipChest = EquipChest2;
    DoorOpen = DoorOpen2;
    EquipFeet = EquipFeet2;
    EquipHead = EquipHead2;
    Walk = Walk2;
    MaceHit = MaceHit2;
    NeckSound = NeckSound2;
    PotionSound = PotionSound2;
    ScrollSound = ScrollSound2;
    SwordHit = SwordHit2;
    Drink = Drink2;
    Miss = Miss2;
    Drop = Drop2;
    ProjectileHit = ProjectileHit2;
    EquipWeapon = EquipWeapon2;
    EquipShield = EquipShield2;
    EquipRing = EquipRing2;
    EquipNeck = EquipNeck2;
    WeaponSound = WeaponSound2;
    ShieldSound = ShieldSound2;
    HeadSound = HeadSound2;
    ChestSound = ChestSound2;
    FeetSound = FeetSound2;
    RingSound = RingSound2;
    TurnPage = TurnPage2;
  } else {
    EquipChest = EquipChest1;
    DoorOpen = DoorOpen1;
    EquipFeet = EquipFeet1;
    EquipHead = EquipHead1;
    Walk = Walk1;
    MaceHit = MaceHit1;
    NeckSound = NeckSound1;
    PotionSound = PotionSound1;
    ScrollSound = ScrollSound1;
    SwordHit = SwordHit1;
    Drink = Drink1;
    Miss = Miss1;
    Drop = Drop1;
    ProjectileHit = ProjectileHit1;
    EquipWeapon = EquipWeapon1;
    EquipShield = EquipShield1;
    EquipRing = EquipRing1;
    EquipNeck = EquipNeck1;
    WeaponSound = WeaponSound1;
    ShieldSound = ShieldSound1;
    HeadSound = HeadSound1;
    ChestSound = ChestSound1;
    FeetSound = FeetSound1;
    RingSound = RingSound1;
    TurnPage = TurnPage1;
  }
}

void CMusic::ToggleMusic()
{
  music_on_ = !music_on_;
  if (music_on_) {
    if (Mix_PlayingMusic() == 0) {
      //if (Music) CMusic::SoundControl.Play(Music, -1);
    } else {
      if (Mix_PausedMusic()) {
        Mix_ResumeMusic();
      }
    }
  } else {
    if (Mix_PlayingMusic()) {
      if (Mix_PausedMusic() == 0) {
        Mix_PauseMusic();
      }
    }
  }
}

void CMusic::ToggleSound()
{
  sound_on_ = !sound_on_;
}

void CMusic::OnCleanup()
{
  
  if (EquipChest1) Mix_FreeChunk(EquipChest1);
  if (EquipChest2) Mix_FreeChunk(EquipChest2);
  
  if (DoorOpen1) Mix_FreeChunk(DoorOpen1);
  if (DoorOpen2) Mix_FreeChunk(DoorOpen2);
  
  if (EquipFeet1) Mix_FreeChunk(EquipFeet1);
  if (EquipFeet2) Mix_FreeChunk(EquipFeet2);
  
  if (EquipHead1) Mix_FreeChunk(EquipHead1);
  if (EquipHead2) Mix_FreeChunk(EquipHead2);
  
  if (Walk1) Mix_FreeChunk(Walk1);
  if (Walk2) Mix_FreeChunk(Walk2);
  
  if (MaceHit1) Mix_FreeChunk(MaceHit1);
  if (MaceHit2) Mix_FreeChunk(MaceHit2);
  
  if (NeckSound1) Mix_FreeChunk(NeckSound1);
  if (NeckSound2) Mix_FreeChunk(NeckSound2);
  
  if (PotionSound1) Mix_FreeChunk(PotionSound1);
  if (PotionSound2) Mix_FreeChunk(PotionSound2);
  
  if (ScrollSound1) Mix_FreeChunk(ScrollSound1);
  if (ScrollSound2) Mix_FreeChunk(ScrollSound2);
  
  if (SwordHit1) Mix_FreeChunk(SwordHit1);
  if (SwordHit2) Mix_FreeChunk(SwordHit2);
  
  if (LevelUp) Mix_FreeChunk(LevelUp);
  
  if (Descend) Mix_FreeChunk(Descend);
  
  if (Drink1) Mix_FreeChunk(Drink1);
  if (Drink2) Mix_FreeChunk(Drink2);
  
  if (Miss1) Mix_FreeChunk(Miss1);
  if (Miss2) Mix_FreeChunk(Miss2);
  
  if (Drop1) Mix_FreeChunk(Drop1);
  if (Drop2) Mix_FreeChunk(Drop2);
  
  if (ProjectileHit1) Mix_FreeChunk(ProjectileHit1);
  if (ProjectileHit2) Mix_FreeChunk(ProjectileHit2);
  
  if (EquipWeapon1) Mix_FreeChunk(EquipWeapon1);
  if (EquipWeapon2) Mix_FreeChunk(EquipWeapon2);

  if (EquipShield1) Mix_FreeChunk(EquipShield1);  
  if (EquipShield2) Mix_FreeChunk(EquipShield2);
  
  if (EquipRing1) Mix_FreeChunk(EquipRing1);
  if (EquipRing2) Mix_FreeChunk(EquipRing2);
  
  if (EquipNeck1) Mix_FreeChunk(EquipNeck1);
  if (EquipNeck2) Mix_FreeChunk(EquipNeck2);
  
  if (WeaponSound1) Mix_FreeChunk(WeaponSound1);
  if (WeaponSound2) Mix_FreeChunk(WeaponSound2);
  
  if (ShieldSound1) Mix_FreeChunk(ShieldSound1);
  if (ShieldSound2) Mix_FreeChunk(ShieldSound2);
  
  if (HeadSound1) Mix_FreeChunk(HeadSound1);
  if (HeadSound2) Mix_FreeChunk(HeadSound2);
  
  if (ChestSound1) Mix_FreeChunk(ChestSound1);
  if (ChestSound2) Mix_FreeChunk(ChestSound2);
  
  if (FeetSound1) Mix_FreeChunk(FeetSound1);
  if (FeetSound2) Mix_FreeChunk(FeetSound2);
  
  if (RingSound1) Mix_FreeChunk(RingSound1);
  if (RingSound2) Mix_FreeChunk(RingSound2);
  
  if (TurnPage1) Mix_FreeChunk(TurnPage1);
  if (TurnPage2) Mix_FreeChunk(TurnPage2);
}
