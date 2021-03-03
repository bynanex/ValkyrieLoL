#include "Offsets.h"

Offsets::Offsets() {};

std::string Offsets::GameVersion             = "11.4";

int Offsets::ObjectManager                   = 0x016dc5f4;
int Offsets::Renderer                        = 0x02f9e33c;
int Offsets::ViewMatrix                      = 0x02f9b4e0;
int Offsets::MinimapObject                   = 0x02f78b48;
int Offsets::LocalPlayer                     = 0x02f792b8;
int Offsets::GameTime                        = 0x02f71134;
int Offsets::Chat                            = 0x02f79384;
int Offsets::FnCharacterDataStackUpdate      = 0x000f3850;

int Offsets::UnderMouseObject                = 0x232A7C0;
int Offsets::ProjectionMatrix                = ViewMatrix + 16*sizeof(float);

int Offsets::ChatIsOpen                      = 0x650;

int Offsets::ObjIndex                        = 0x20;
int Offsets::ObjTeam                         = 0x4C;
int Offsets::ObjMissileName                  = 0x6C;
int Offsets::ObjNetworkID                    = 0xCC;
int Offsets::ObjPos                          = 0x1d8;
int Offsets::ObjMissileSpellCast             = 0x228;
int Offsets::ObjVisibility                   = 0x270;
int Offsets::ObjSpawnCount                   = 0x284;
int Offsets::ObjMana                         = 0x298;
int Offsets::ObjRecallState                  = 0xD8C;
int Offsets::ObjHealth                       = 0xD98;
int Offsets::ObjMaxHealth                    = 0xDA8;
int Offsets::ObjArmor                        = 0x129C;
int Offsets::ObjMagicRes                     = 0x12A4;
int Offsets::ObjBaseAtk                      = 0x1274;
int Offsets::ObjBonusAtk                     = 0x11F0;
int Offsets::ObjMoveSpeed                    = 0x12B4;
int Offsets::ObjSpellBook                    = 0x2720;
int Offsets::ObjTransformation               = 0x2f78;
int Offsets::ObjName                         = 0x2F84;
int Offsets::ObjLvl                          = 0x36D4;
int Offsets::ObjExpiry                       = 0x298; 
int Offsets::ObjCrit                         = 0x1298;
int Offsets::ObjCritMulti                    = 0x1288;
int Offsets::ObjAbilityPower                 = 0x1200;
int Offsets::ObjAtkSpeedMulti                = 0x1270;
int Offsets::ObjAtkRange                     = 0x12BC;
int Offsets::ObjTargetable                   = 0xD00;
int Offsets::ObjInvulnerable                 = 0x3D0; 

int Offsets::ObjBuffManager                  = 0x2178;
int Offsets::BuffManagerEntriesArray         = 0x10;
int Offsets::BuffEntryBuff                   = 0x8;
int Offsets::BuffEntryBuffStartTime          = 0xC;
int Offsets::BuffEntryBuffEndTime            = 0x10;
int Offsets::BuffName                        = 0x8;
//oObjIsMoving = 363B;
//oObjDirection = 0x1B80;
//oObjActionState = 0x1054;

int Offsets::SpellBookActiveSpellCast        = 0x20;
int Offsets::SpellBookSpellSlots             = 0x478;

int Offsets::SpellCastSpellInfo              = 0x8;
int Offsets::SpellCastStartTime              = 0x544;
int Offsets::SpellCastCastTime               = 0x4C0;
int Offsets::SpellCastStart                  = 0x80;
int Offsets::SpellCastEnd                    = 0x8C;
int Offsets::SpellCastSrcIdx                 = 0x68;
int Offsets::SpellCastDestIdx                = 0xC0;

int Offsets::ObjItemList                     = 0x3720;
int Offsets::ItemListItem                    = 0xC;
int Offsets::ItemInfo                        = 0x20;
int Offsets::ItemInfoId                      = 0x68;
										    
int Offsets::RendererWidth                   = 0x10;
int Offsets::RendererHeight                  = 0x14;
										    
int Offsets::SpellSlotLevel                  = 0x20;
int Offsets::SpellSlotTime                   = 0x28;
int Offsets::SpellSlotValue                  = 0x94;
int Offsets::SpellSlotSpellInfo              = 0x13C;
int Offsets::SpellInfoSpellData              = 0x44;
int Offsets::SpellDataSpellName              = 0x64;
int Offsets::SpellDataMissileName            = 0x64;
										    
int Offsets::ObjectMapCount                  = 0x2C;
int Offsets::ObjectMapRoot                   = 0x28;
int Offsets::ObjectMapNodeNetId              = 0x10;
int Offsets::ObjectMapNodeObject             = 0x14;

int Offsets::MinimapObjectHud                = 0x88;
int Offsets::MinimapHudPos                   = 0x60;
int Offsets::MinimapHudSize                  = 0x68;          // has values between (191, 191) and (383, 383)
										    
int Offsets::CharacterDataStack              = 0x2F78;
int Offsets::CharacterDataStackSkinId        = 0x18;