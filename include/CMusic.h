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
#ifndef _CMUSIC_H_
#define _CMUSIC_H_

#include <SDL_mixer.h>

/*
 * Class for music, handles playing and stopping music.
 * Currently stores all sounds for the game, this should be
 * redesigned for easier extension.
 */
class CMusic {
  public:
    static CMusic SoundControl;
  
    Mix_Chunk *EquipChest1;
    Mix_Chunk *EquipChest2;
    Mix_Chunk *EquipChest;
    
    Mix_Chunk *DoorOpen1;
    Mix_Chunk *DoorOpen2;
    Mix_Chunk *DoorOpen;
    
    Mix_Chunk *EquipFeet1;
    Mix_Chunk *EquipFeet2;
    Mix_Chunk *EquipFeet;
    
    Mix_Chunk *EquipHead1;
    Mix_Chunk *EquipHead2;
    Mix_Chunk *EquipHead;
    
    Mix_Chunk *Walk1;
    Mix_Chunk *Walk2;
    Mix_Chunk *Walk;
    
    Mix_Chunk *MaceHit1;
    Mix_Chunk *MaceHit2;
    Mix_Chunk *MaceHit;
    
    Mix_Chunk *NeckSound1; //Necklace/amulet pickup
    Mix_Chunk *NeckSound2;
    Mix_Chunk *NeckSound;
    
    Mix_Chunk *PotionSound1;
    Mix_Chunk *PotionSound2;
    Mix_Chunk *PotionSound;
    
    Mix_Chunk *ScrollSound1;
    Mix_Chunk *ScrollSound2;
    Mix_Chunk *ScrollSound;
    
    Mix_Chunk *SwordHit1;
    Mix_Chunk *SwordHit2;
    Mix_Chunk *SwordHit;
    
    Mix_Chunk *LevelUp;
    
    Mix_Chunk *Descend;
    
    Mix_Chunk *Drink1;
    Mix_Chunk *Drink2;
    Mix_Chunk *Drink;
    
    Mix_Chunk *Miss1;
    Mix_Chunk *Miss2;
    Mix_Chunk *Miss;
    
    Mix_Chunk *Drop1;
    Mix_Chunk *Drop2;
    Mix_Chunk *Drop;
    
    Mix_Chunk *ProjectileHit1;
    Mix_Chunk *ProjectileHit2;
    Mix_Chunk *ProjectileHit;
    
    Mix_Chunk *EquipWeapon1;
    Mix_Chunk *EquipWeapon2;
    Mix_Chunk *EquipWeapon;
    
    Mix_Chunk *EquipShield1;
    Mix_Chunk *EquipShield2;
    Mix_Chunk *EquipShield;
    
    Mix_Chunk *EquipRing1;
    Mix_Chunk *EquipRing2;
    Mix_Chunk *EquipRing;
    
    Mix_Chunk *EquipNeck1;
    Mix_Chunk *EquipNeck2;
    Mix_Chunk *EquipNeck;
    
    Mix_Chunk *WeaponSound1;
    Mix_Chunk *WeaponSound2;
    Mix_Chunk *WeaponSound;
    
    Mix_Chunk *ShieldSound1;
    Mix_Chunk *ShieldSound2;
    Mix_Chunk *ShieldSound;
    
    Mix_Chunk *HeadSound1;
    Mix_Chunk *HeadSound2;
    Mix_Chunk *HeadSound;
    
    Mix_Chunk *ChestSound1;
    Mix_Chunk *ChestSound2;
    Mix_Chunk *ChestSound;
    
    Mix_Chunk *FeetSound1;
    Mix_Chunk *FeetSound2;
    Mix_Chunk *FeetSound;
    
    Mix_Chunk *RingSound1;
    Mix_Chunk *RingSound2;
    Mix_Chunk *RingSound;
    
    Mix_Chunk *TurnPage1;
    Mix_Chunk *TurnPage2;
    Mix_Chunk *TurnPage;
  
    CMusic();

    static Mix_Music *OnLoad(const char *file);

    bool Play(Mix_Music *music, int loop_number);
    void ChangeMusic(Mix_Music *music);
    bool LoadSounds();
    void OnCleanup();
    // Changes the next sound to play
    // (prevents from 2 same sounds happening - there are 2 for each acion)
    void SoundVariance();
    void ToggleMusic();
    void ToggleSound();
    void PlaySFX(Mix_Chunk *sound, int loop_number);

  private:
    bool music_on_;
    bool sound_on_;
};

#endif
