// Automatisch erzeugt von tools/types/gen_types.py aus symbols/gc_types.h (WP-04).
// PC-Layout (MSVC, 32 Bit): bool = 1 Byte, Vptr am Anfang der einführenden Klasse, Basisklasse als Feld `base`.
// Offsets und Größen sind berechnet; Abweichungen mit Code-Beleg in tools/types/pc_overrides.py.
// Nicht von Hand bearbeiten.
#pragma once

enum ARAM_DIRECTION {
    ARAM_LOW = 0,
    ARAM_HIGH = 256,
};
typedef enum ARAM_DIRECTION ARAM_DIRECTION;
enum Axis {
    X_AXIS = 0,
    Y_AXIS = 4,
    Z_AXIS = 8,
};
typedef enum Axis Axis;
enum CHARTYPE {
    CH_REGULAR = 0,
    CH_PERCENT = 1,
    CH_DOT = 2,
    CH_STAR = 3,
    CH_ZERO = 4,
    CH_DIGIT = 5,
    CH_FLAG = 6,
    CH_SIZE = 7,
    CH_TYPE = 8,
};
typedef enum CHARTYPE CHARTYPE;
enum CarEffectPosition {
    CARFXPOS_NONE = 0,
    CARFXPOS_FRONT_TIRES = 1,
    CARFXPOS_REAR_TIRES = 2,
    CARFXPOS_LEFT_TIRES = 3,
    CARFXPOS_RIGHT_TIRES = 4,
    CARFXPOS_TIRE_FL = 5,
    CARFXPOS_TIRE_FR = 6,
    CARFXPOS_TIRE_RR = 7,
    CARFXPOS_TIRE_RL = 8,
    CARFXPOS_ENGINE = 9,
    CARFXPOS_EXHAUST = 10,
    CARFXPOS_RADIATOR = 11,
    CARFXPOS_WINDSHIELD = 12,
    CARFXPOS_WINDOW_REAR = 13,
    CARFXPOS_WINDOW_LEFT_FRONT = 14,
    CARFXPOS_WINDOW_LEFT_REAR = 15,
    CARFXPOS_WINDOW_RIGHT_FRONT = 16,
    CARFXPOS_WINDOW_RIGHT_REAR = 17,
    CARFXPOS_HEADLIGHT = 18,
    CARFXPOS_BRAKELIGHT = 19,
    CARFXPOS_HEADLIGHT_LEFT = 20,
    CARFXPOS_HEADLIGHT_RIGHT = 21,
    CARFXPOS_BRAKELIGHT_LEFT = 22,
    CARFXPOS_BRAKELIGHT_RIGHT = 23,
    CARFXPOS_SIDE_MIRROR_LEFT = 24,
    CARFXPOS_SIDE_MIRROR_RIGHT = 25,
    CARFXPOS_LICENSE_PLATE_FRONT = 26,
    CARFXPOS_LICENSE_PLATE_REAR = 27,
    NUM_CARFXPOS = 28,
};
typedef enum CarEffectPosition CarEffectPosition;
enum ConditionForceNumber {
    CONDITION_0 = 0,
    CONDITION_1 = 1,
    CONDITION_2 = 2,
    CONDITION_3 = 3,
    CONDITION_4 = 4,
    CONDITION_5 = 5,
    CONDITION_6 = 6,
    CONDITION_7 = 7,
};
typedef enum ConditionForceNumber ConditionForceNumber;
enum ConstantForceNumber {
    CONSTANT_0 = 0,
    CONSTANT_1 = 1,
    CONSTANT_2 = 2,
    CONSTANT_3 = 3,
    CONSTANT_4 = 4,
    CONSTANT_5 = 5,
    CONSTANT_6 = 6,
    CONSTANT_7 = 7,
};
typedef enum ConstantForceNumber ConstantForceNumber;
enum DEMOMemStatArg {
    DEMO_STAT_MEM_CP = 0,
    DEMO_STAT_MEM_TC = 1,
    DEMO_STAT_MEM_CPUR = 2,
    DEMO_STAT_MEM_CPUW = 3,
    DEMO_STAT_MEM_DSP = 4,
    DEMO_STAT_MEM_IO = 5,
    DEMO_STAT_MEM_VI = 6,
    DEMO_STAT_MEM_PE = 7,
    DEMO_STAT_MEM_RF = 8,
    DEMO_STAT_MEM_FI = 9,
};
typedef enum DEMOMemStatArg DEMOMemStatArg;
enum DEMOPixStatArg {
    DEMO_STAT_PIX_TI = 0,
    DEMO_STAT_PIX_TO = 1,
    DEMO_STAT_PIX_BI = 2,
    DEMO_STAT_PIX_BO = 3,
    DEMO_STAT_PIX_CI = 4,
    DEMO_STAT_PIX_CC = 5,
};
typedef enum DEMOPixStatArg DEMOPixStatArg;
enum DEMOStatDispMode {
    DEMO_STAT_TL = 0,
    DEMO_STAT_BL = 1,
    DEMO_STAT_TLD = 2,
    DEMO_STAT_BLD = 3,
    DEMO_STAT_IO = 4,
};
typedef enum DEMOStatDispMode DEMOStatDispMode;
enum DEMOStatType {
    DEMO_STAT_GP0 = 0,
    DEMO_STAT_GP1 = 1,
    DEMO_STAT_MEM = 2,
    DEMO_STAT_PIX = 3,
    DEMO_STAT_VC = 4,
    DEMO_STAT_FR = 5,
    DEMO_STAT_TBW = 6,
    DEMO_STAT_TBP = 7,
    DEMO_STAT_MYC = 8,
    DEMO_STAT_MYR = 9,
};
typedef enum DEMOStatType DEMOStatType;
enum DEMOVcStatArg {
    DEMO_STAT_VC_CHK = 0,
    DEMO_STAT_VC_MISS = 1,
    DEMO_STAT_VC_STALL = 2,
};
typedef enum DEMOVcStatArg DEMOVcStatArg;
enum DMTexFlt {
    DMTF_POINTSAMPLE = 0,
    DMTF_BILERP = 1,
};
typedef enum DMTexFlt DMTexFlt;
enum DemoDiscEndReason {
    DEMO_DISC_ENDREASON_NONE = -1,
    DEMO_DISC_ENDREASON_ATTRACT_INTERRUPTED = 0,
    DEMO_DISC_ENDREASON_ATTRACT_COMPLETE = 1,
    DEMO_DISC_ENDREASON_PLAYABLE_INACTIVITY_TIMEOUT = 2,
    DEMO_DISC_ENDREASON_PLAYABLE_GAMEPLAY_TIMEOUT = 3,
    DEMO_DISC_ENDREASON_PLAYABLE_COMPLETE = 4,
    DEMO_DISC_ENDREASON_PLAYABLE_QUIT = 5,
};
typedef enum DemoDiscEndReason DemoDiscEndReason;
enum ENVMAP_FACES {
    ENVMAP_F = 0,
    ENVMAP_R = 1,
    ENVMAP_B = 2,
    ENVMAP_L = 3,
    ENVMAP_U = 4,
    ENVMAP_D = 5,
    ENVMAP_NUM_FACES = 6,
};
typedef enum ENVMAP_FACES ENVMAP_FACES;
enum EVIEWMODE {
    EVIEWMODE_NONE = 0,
    EVIEWMODE_ONE = 1,
    EVIEWMODE_ONE_RVM = 2,
    EVIEWMODE_TWOH = 3,
    EVIEWMODE_TWOV = 4,
};
typedef enum EVIEWMODE EVIEWMODE;
enum EVIEW_CLIP_PLANE {
    EVIEW_LEFT = 0,
    EVIEW_RIGHT = 1,
    EVIEW_BOTTOM = 2,
    EVIEW_TOP = 3,
    EVIEW_NEAR = 4,
    EVIEW_FAR = 5,
    EVIEW_CLIP_FIRST = 0,
    EVIEW_CLIP_LAST = 5,
    EVIEW_CLIP_NUM = 6,
};
typedef enum EVIEW_CLIP_PLANE EVIEW_CLIP_PLANE;
enum EVIEW_ID {
    EVIEW_NONE = -1,
    EVIEW_FLAILER = 0,
    EVIEW_PLAYER1 = 1,
    EVIEW_PLAYER2 = 2,
    EVIEW_PLAYER1_RVM = 3,
    EVIEW_PLAYER1_REFLECTION = 4,
    EVIEW_PLAYER2_REFLECTION = 5,
    EVIEW_PLAYER1_GLOW = 6,
    EVIEW_PLAYER2_GLOW = 7,
    EVIEW_HEADLIGHT_P1 = 8,
    EVIEW_HEADLIGHT_P2 = 9,
    EVIEW_ENVMAP0F = 10,
    EVIEW_ENVMAP0R = 11,
    EVIEW_ENVMAP0B = 12,
    EVIEW_ENVMAP0L = 13,
    EVIEW_ENVMAP0U = 14,
    EVIEW_ENVMAP0D = 15,
    EVIEW_FIRST = 0,
    EVIEW_LAST = 15,
    EVIEW_FIRST_RENDER = 1,
    EVIEW_LAST_RENDER = 15,
    EVIEW_FIRST_PLAYER = 1,
    EVIEW_LAST_PLAYER = 3,
    EVIEW_FIRST_RVM = 3,
    EVIEW_LAST_RVM = 3,
    EVIEW_FIRST_ENVMAP = 10,
    EVIEW_LAST_ENVMAP = 15,
    NUM_EVIEWS = 16,
    NUM_RENDER_VIEWS = 15,
    NUM_PLAYER_VIEWS = 3,
    NUM_RVM_VIEWS = 1,
};
typedef enum EVIEW_ID EVIEW_ID;
enum EasterEggButtons {
    L1 = 173,
    L2 = 174,
    R1 = 175,
    R2 = 176,
    LT = 177,
    RT = 178,
    SQ = 179,
    TR = 180,
    UP = 183,
    DN = 184,
    XX = 0,
};
typedef enum EasterEggButtons EasterEggButtons;
enum EasterEggGroups {
    EASTER_EGG_NO_GROUP = 0,
    EASTER_EGG_RENDERING_GROUP = 1,
};
typedef enum EasterEggGroups EasterEggGroups;
enum EasterEggsSpecial {
    EASTER_EGG_SPECIAL_LANGUAGES_TOGGLE = 0,
    EASTER_EGG_SPECIAL_XBOX_SAFEZONE = 1,
};
typedef enum EasterEggsSpecial EasterEggsSpecial;
enum EasterEggsType {
    EASTER_EGG_CARS = 0,
    EASTER_EGG_VISUAL_PARTS = 1,
    EASTER_EGG_PERF_PARTS = 2,
    EASTER_EGG_DRIFT_PHYSICS = 3,
    EASTER_EGG_TRACK = 4,
    EASTER_EGG_SPECIAL = 5,
    EASTER_EGG_PRESETCARS = 6,
};
typedef enum EasterEggsType EasterEggsType;
enum EspressoFunctions {
    EF_IsCaffeineRunning = 0,
    EF_InitCommunication = 1,
    EF_AddMessage = 2,
    EF_CloseCaffeine = 3,
    EF_LoadWorld = 4,
    EF_ReloadWorld = 5,
    EF_SaveWorld = 6,
    EF_EmptyLayer = 7,
    EF_GetFirstObject = 8,
    EF_GetNextObject = 9,
    EF_GetLayerState = 10,
    EF_IsLayerReadOnly = 11,
    EF_GetLayerHandle = 12,
    EF_CreateObject = 13,
    EF_CreateObjectAsync = 14,
    EF_DeleteObject = 15,
    EF_GetObjectType = 16,
    EF_GetObjectLayer = 17,
    EF_GetObjectPosition = 18,
    EF_SetObjectPosition = 19,
    EF_GetObjectPositionFix = 20,
    EF_SetObjectPositionFix = 21,
    EF_GetObjectMatrix = 22,
    EF_SetObjectMatrix = 23,
    EF_GetMeshName = 24,
    EF_SetMeshName = 25,
    EF_CreateUserMesh = 26,
    EF_SetUserMeshFace = 27,
    EF_SetSphereRadius = 28,
    EF_LinkObject = 29,
    EF_UnlinkObject = 30,
    EF_GetLink = 31,
    EF_GetNumReverseLinks = 32,
    EF_GetReverseLink = 33,
    EF_IsAttributeSet = 34,
    EF_GetAttributeString = 35,
    EF_GetAttributeInt = 36,
    EF_GetAttributeFloat = 37,
    EF_SetAttributeString = 38,
    EF_SetAttributeInt = 39,
    EF_SetAttributeFloat = 40,
    EF_GetAttributeHandle = 41,
    EF_GetString = 42,
    EF_SetString = 43,
    EF_GetLong = 44,
    EF_SetLong = 45,
    EF_GetFloat = 46,
    EF_SetFloat = 47,
    EF_IsObjectSelected = 48,
    EF_SelectObject = 49,
    EF_UnselectObject = 50,
    EF_UnselectAllObjects = 51,
    EF_GetNumSelectedObjects = 52,
    EF_GetSelectedObject = 53,
    EF_GetCameraPosition = 54,
    EF_SetCameraPosition = 55,
    EF_GetCameraPositionFix = 56,
    EF_SetCameraPositionFix = 57,
    EF_GetCursorPosition = 58,
    EF_SetCursorPosition = 59,
};
typedef enum EspressoFunctions EspressoFunctions;
enum ExportedVariableFlags {
    VAR_VOLATILE = 1,
    VAR_CPUVARIABLE = 2,
};
typedef enum ExportedVariableFlags ExportedVariableFlags;
enum FEButtonMap_Directions {
    FEBM_Dir_Up = 0,
    FEBM_Dir_UpRight = 1,
    FEBM_Dir_Right = 2,
    FEBM_Dir_DownRight = 3,
    FEBM_Dir_Down = 4,
    FEBM_Dir_DownLeft = 5,
    FEBM_Dir_Left = 6,
    FEBM_Dir_UpLeft = 7,
};
typedef enum FEButtonMap_Directions FEButtonMap_Directions;
enum FEButtonWrapMode {
    Wrap_None = 0,
    Wrap_Horizontal = 1,
    Wrap_Vertical = 2,
    Wrap_Both = 3,
};
typedef enum FEButtonWrapMode FEButtonWrapMode;
enum FEInterpMethod {
    IT_None = 0,
    IT_Linear = 1,
    IT_Spline = 2,
    IT_MoveToLinear = 3,
    IT_MoveToSpline = 4,
    IT_InterpTypeCount = 5,
};
typedef enum FEInterpMethod FEInterpMethod;
enum FEKeyTrack_Indices {
    FETrack_Color = 0,
    FETrack_Pivot = 1,
    FETrack_Position = 2,
    FETrack_Rotation = 3,
    FETrack_Size = 4,
    FETrack_UpperLeft = 5,
    FETrack_LowerRight = 6,
    FETrack_FrameNumber = 7,
    FETrack_Color1 = 7,
    FETrack_Color2 = 8,
    FETrack_Color3 = 9,
    FETrack_Color4 = 10,
    Num_BaseFETracks = 11,
};
typedef enum FEKeyTrack_Indices FEKeyTrack_Indices;
enum FEMessageResponseCommands {
    MR_SetScript = 0,
    MR_PostMessageToFEng = 1,
    MR_PostMessageToGame = 2,
    MR_PostMessageToSound = 3,
    MR_SetActiveButton = 256,
    MR_SetInputProcessing = 257,
    MR_RecordCurrentButton = 258,
    MR_RecallRecordedButton = 259,
    MR_DontNavigate = 260,
    MR_PassControlToChildCurrent = 261,
    MR_PassControlToChildGlobal = 262,
    MR_PassControlToParentCurrent = 263,
    MR_PassControlToParentGlobal = 264,
    MR_SwitchToPackage = 512,
    MR_PushPackageGlobal = 513,
    MR_PushPackageCurrent = 514,
    MR_PopPackage = 515,
    MR_PushPackageNone = 516,
    MR_RecordPackageMarker = 704,
    MR_SwitchToPackageMarker = 705,
    MR_ClearPackageMarkers = 706,
    MR_IfScriptEquals = 768,
    MR_IfScriptNotEquals = 769,
    MR_Else = 1280,
    MR_EndIf = 1281,
};
typedef enum FEMessageResponseCommands FEMessageResponseCommands;
enum FEObjType {
    FE_None = 0,
    FE_Image = 1,
    FE_String = 2,
    FE_Model = 3,
    FE_List = 4,
    FE_Group = 5,
    FE_CodeList = 6,
    FE_Movie = 7,
    FE_Effect = 8,
    FE_ColoredImage = 9,
    FE_AnimImage = 10,
    FE_SimpleImage = 11,
    FE_UserMin = 256,
};
typedef enum FEObjType FEObjType;
enum FEObject_Flags {
    FF_HideInEdit = -2147483648,
    FF_AffectAllScripts = 1073741824,
    FF_PerspectiveProjection = 536870912,
    FF_IsButton = 268435456,
    FF_ObjectLocked = 134217728,
    FF_IgnoreButton = 67108864,
    FF_Dirty = 33554432,
    FF_DirtyTransform = 16777216,
    FF_DirtyColor = 8388608,
    FF_DirtyCode = 4194304,
    FF_CodeSuppliedResource = 2097152,
    FF_UsesLibraryObject = 1048576,
    FF_DontNavigate = 524288,
    FF_SaveStaticTracks = 262144,
};
typedef enum FEObject_Flags FEObject_Flags;
enum FEParamType {
    PT_Bool = 0,
    PT_Int = 1,
    PT_Float = 2,
    PT_Vector2 = 3,
    PT_Vector3 = 4,
    PT_Quaternion = 5,
    PT_Color = 6,
    PT_ParamTypeCount = 7,
};
typedef enum FEParamType FEParamType;
enum FEPlayActions {
    AT_Once = 0,
    AT_Loop = 1,
    AT_PingPong = 2,
    AT_ActionTypeCount = 3,
};
typedef enum FEPlayActions FEPlayActions;
enum FEResourceType {
    RT_None = 0,
    RT_Image = 1,
    RT_Font = 2,
    RT_Model = 3,
    RT_Movie = 4,
    RT_Effect = 5,
    RT_AnimImage = 6,
    RT_ResourceTypeCount = 7,
};
typedef enum FEResourceType FEResourceType;
enum FEStringFormatting {
    _FESTRING_JUSTIFY_HCENTER = 0,
    _FESTRING_JUSTIFY_HRIGHT = 1,
    _FESTRING_JUSTIFY_VCENTER = 2,
    _FESTRING_JUSTIFY_VBOTTOM = 3,
    _FESTRING_FORMAT_WORDWRAP = 4,
    FESTRING_JUSTIFY_FORCEU32 = -1,
};
typedef enum FEStringFormatting FEStringFormatting;
enum FILEERR {
    FERR_NormalEnd = 0,
    FERR_NoOps = 1,
    FERR_NoHandles = 2,
    FERR_BigClose = 3,
    FERR_BigOpen = 4,
    FERR_NoExist = 5,
    FERR_InvalidHandle = 6,
    FERR_NumErrors = 7,
};
typedef enum FILEERR FILEERR;
enum FILEOPTYPE {
    FOPTYPE_unused = 0,
    FOPTYPE_used = 1,
    FOPTYPE_open = 2,
    FOPTYPE_close = 3,
    FOPTYPE_read = 4,
    FOPTYPE_write = 5,
    FOPTYPE_size = 6,
    FOPTYPE_null = 7,
    FOPTYPE_exist = 8,
    FOPTYPE_addbig = 9,
    FOPTYPE_delbig = 10,
    FOPTYPE_delete = 11,
};
typedef enum FILEOPTYPE FILEOPTYPE;
enum FILE_TypeEnum {
    FREE_FILE = 0,
    BIG_FILE = 1,
    DVD_FILE = 2,
    HOST_FILE = 3,
};
typedef enum FILE_TypeEnum FILE_TypeEnum;
enum FILTER_ID {
    FILTER_DEFAULT = 0,
    FILTER_EFB_XFB = 1,
    FILTER_CUBE_FACES = 2,
    FILTER_SPHERE_MAP = 3,
    FILTER_REFLECTION = 4,
    FILTER_MOTIONBLUR = 5,
    FILTER_GLOWBLOOM = 6,
    FILTER_TOTAL = 7,
};
typedef enum FILTER_ID FILTER_ID;
enum FONTVALUE {
    FONT_FALSE = 0,
    FONT_TRUE = 1,
    FONT_FILTER_POINT = 0,
    FONT_FILTER_BILINEAR = 1,
    FONT_DEPTHWRITE_DISABLE = 0,
    FONT_DEPTHWRITE_ENABLE = 1,
};
typedef enum FONTVALUE FONTVALUE;
enum FS_EVNT {
    FS_EVNT_AS_CMDCOMPL = 0,
    FS_EVNT_OPEN = 1,
    FS_EVNT_CLOSE = 2,
    FS_EVNT_AS_READ = 3,
    FS_EVNT_AS_WRITE = 4,
    FS_EVNT_AS_EXIST = 5,
    FS_EVNT_AS_ADDBIG = 6,
    FS_EVNT_CD_READ = 7,
    FS_EVNT_CD_READCOMPL = 8,
    FS_EVNT_CACHEFILE = 9,
    FS_EVNT_CACHEWRITE = 10,
    FS_EVNT_CACHEHIT = 11,
};
typedef enum FS_EVNT FS_EVNT;
enum FileReadStateEnum {
    START = 0,
    SMALL_FILE = 1,
    ALIGN_THE_START = 2,
    ALIGN_READ = 3,
    NONALIGN_READ = 4,
    DONE = 5,
};
typedef enum FileReadStateEnum FileReadStateEnum;
enum FogPalleteModes {
    FOG_NORMAL_P = 0,
    FOG_NIGHT_P = 1,
    FOG_DEPTH_P = 2,
    FOG_SINGLE_P = 3,
    FOG_VOLUME_P = 4,
    FOG_HEADLIGHT_P = 5,
    FOG_NUM_MODES = 6,
};
typedef enum FogPalleteModes FogPalleteModes;
enum FogVolumeBlend {
    FOG_BLEND_NORMAL = 0,
    FOG_BLEND_ADDATIVE = 1,
    FOG_BLEND_SUBTRACTIVE = 2,
};
typedef enum FogVolumeBlend FogVolumeBlend;
enum FogVolumeShape {
    SPHERE = 0,
    HALF_SPHERE = 1,
    CUBE = 2,
    CYLINDER = 3,
    CONE = 4,
    FOG_NUM_SHAPES = 5,
};
typedef enum FogVolumeShape FogVolumeShape;
enum FogVolumeType {
    DISTANT_FOG = 0,
    VOLUME_FOG = 1,
    HEADLIGHT_FOG = 2,
};
typedef enum FogVolumeType FogVolumeType;
enum ForceType {
    SPRING = 0,
    CONSTANT = 1,
    DAMPER = 2,
    SIDE_COLLISION = 3,
    FRONTAL_COLLISION = 4,
    DIRT_ROAD = 5,
    BUMPY_ROAD = 6,
    SLIPPERY_ROAD = 7,
    SURFACE_EFFECT = 8,
    CAR_AIRBORNE = 9,
    NUMBER_FORCE_EFFECTS = 10,
};
typedef enum ForceType ForceType;
enum FrontendSoundEvent {
    FE_SOUND_NONE = 0,
    FE_SOUND_MENU_LEFT = 1,
    FE_SOUND_MENU_RIGHT = 2,
    FE_SOUND_MENU_UP = 3,
    FE_SOUND_MENU_DOWN = 4,
    FE_SOUND_MENU_SELECT = 5,
    FE_SOUND_MENU_BACK = 6,
    FE_SOUND_MENU_START = 7,
    FE_SOUND_BROWSER_LEFT = 8,
    FE_SOUND_BROWSER_RIGHT = 9,
    FE_SOUND_SLIDER_LEFT = 10,
    FE_SOUND_SLIDER_RIGHT = 11,
    FE_SOUND_SUMMON_DIALOG = 12,
    FE_SOUND_SUMMON_HELP = 13,
    FE_SOUND_SUMMON_CUSTOMIZE = 14,
    FE_SOUND_KEYBOARD_LEFT = 15,
    FE_SOUND_KEYBOARD_RIGHT = 16,
    FE_SOUND_KEYBOARD_UP = 17,
    FE_SOUND_KEYBOARD_DOWN = 18,
    FE_SOUND_KEYBOARD_ENTER = 19,
    FE_SOUND_KEYBOARD_DELETE = 20,
};
typedef enum FrontendSoundEvent FrontendSoundEvent;
enum GXWarningLevel {
    GX_WARN_NONE = 0,
    GX_WARN_SEVERE = 1,
    GX_WARN_MEDIUM = 2,
    GX_WARN_ALL = 3,
};
typedef enum GXWarningLevel GXWarningLevel;
enum GameFlowState {
    GAMEFLOW_STATE_NONE = 0,
    GAMEFLOW_STATE_LOADING_FRONTEND = 1,
    GAMEFLOW_STATE_UNLOADING_FRONTEND = 2,
    GAMEFLOW_STATE_IN_FRONTEND = 3,
    GAMEFLOW_STATE_LOADING_TRACK_COMMON = 4,
    GAMEFLOW_STATE_LOADING_TRACK_SPECIFIC = 5,
    GAMEFLOW_STATE_RACING = 6,
    GAMEFLOW_STATE_UNLOADING_TRACK_SPECIFIC = 7,
    GAMEFLOW_STATE_UNLOADING_TRACK_COMMON = 8,
    GAMEFLOW_STATE_EXIT_DEMO_DISC = 9,
};
typedef enum GameFlowState GameFlowState;
enum GamecubeButtonBits {
    BIT_A = 16,
    BIT_B = 17,
    BIT_X = 18,
    BIT_Y = 19,
    BIT_Z = 20,
    BIT_START = 21,
    BIT_DPADU = 24,
    BIT_DPADD = 25,
    BIT_DPADL = 26,
    BIT_DPADR = 27,
    BITS_LTRIGGER = 8,
    BITS_RTRIGGER = 0,
    BITS_ANALOG_LEFT_X = 56,
    BITS_ANALOG_LEFT_Y = 48,
    BITS_ANALOG_RIGHT_X = 40,
    BITS_ANALOG_RIGHT_Y = 32,
};
typedef enum GamecubeButtonBits GamecubeButtonBits;
enum GamecubeControllerModelType {
    PAD_STANDARD = 65,
    PAD_LOGITECH_WHEEL = 80,
    PAD_LOGITECH_WHEEL_PEDALS = 81,
    PAD_NOT_PLUGGED_IN = 255,
};
typedef enum GamecubeControllerModelType GamecubeControllerModelType;
enum GamecubeTerminalType {
    TERM_NONE = 0,
    TERM_DEFAULT = 1,
};
typedef enum GamecubeTerminalType GamecubeTerminalType;
enum ID {
    ID_INVALID = 0,
    ID_FPS = 1,
    ID_MAX = 2,
    ID_USERBASE = 128,
};
typedef enum ID ID;
enum JoyEvent {
    JOY_EVENT_NULL = 0,
    JOY_EVENT_TYPE_CHANGED = 1,
    JOY_EVENT_EXIT_DEMO_DISC = 2,
    JOY_EVENT_SCROLL_UP = 3,
    JOY_EVENT_SCROLL_DOWN = 4,
    JOY_EVENT_SCROLL_LEFT = 5,
    JOY_EVENT_SCROLL_RIGHT = 6,
    JOY_EVENT_START = 7,
    JOY_EVENT_SELECT = 8,
    JOY_EVENT_MENU_SELECT = 9,
    JOY_EVENT_MENU_CANCEL = 10,
    JOY_EVENT_MENU_BACK = 11,
    JOY_EVENT_MENU_HELP = 12,
    JOY_EVENT_MENU_L1 = 13,
    JOY_EVENT_MENU_L2 = 14,
    JOY_EVENT_MENU_R1 = 15,
    JOY_EVENT_MENU_R2 = 16,
    JOY_EVENT_MENU_SQUARE = 17,
    JOY_EVENT_MENU_ANALOG_LEFT = 18,
    JOY_EVENT_MENU_ANALOG_RIGHT = 19,
    JOY_EVENT_MENU_ANALOG_UP = 20,
    JOY_EVENT_MENU_ANALOG_DOWN = 21,
    JOY_EVENT_FENG_START = 22,
    JOY_EVENT_FENG_BUTTON0 = 23,
    JOY_EVENT_FENG_BUTTON1 = 24,
    JOY_EVENT_FENG_BUTTON2 = 25,
    JOY_EVENT_FENG_BUTTON3 = 26,
    JOY_EVENT_FENG_LTRIGGER = 27,
    JOY_EVENT_FENG_RTRIGGER = 28,
    JOY_EVENT_FENG_SELECT = 29,
    JOY_EVENT_FENG_SELECT_ALTERNATE = 30,
    JOY_EVENT_FENG_CANCEL = 31,
    JOY_EVENT_FENG_CANCEL_ALTERNATE = 32,
    JOY_EVENT_FENG_HELP = 33,
    JOY_EVENT_FENG_PAD_UP = 34,
    JOY_EVENT_FENG_PAD_DOWN = 35,
    JOY_EVENT_FENG_PAD_LEFT = 36,
    JOY_EVENT_FENG_PAD_RIGHT = 37,
    JOY_EVENT_FENG_PAD_UP_ALTERNATE = 38,
    JOY_EVENT_FENG_PAD_DOWN_ALTERNATE = 39,
    JOY_EVENT_FENG_PAD_LEFT_ALTERNATE = 40,
    JOY_EVENT_FENG_PAD_RIGHT_ALTERNATE = 41,
    JOY_EVENT_FENG_QUIT = 42,
    JOY_EVENT_FENG_RACEOPTIONS = 43,
    JOY_EVENT_DEBUG_LANGUAGE_CHANGE = 44,
    JOY_EVENT_SKIP_DEMO_MODE = 45,
    JOY_EVENT_MAIN_MENU_SELECT_TRANSMISSION = 46,
    JOY_EVENT_ANY = 47,
    JOY_EVENT_EXIT_EDIT_TEXT = 48,
    JOY_EVENT_CLEAR_TEXT = 49,
    JOY_EVENT_CANCEL_EDIT_TEXT = 50,
    JOY_EVENT_DELETE_CHAR = 51,
    JOY_EVENT_ROTATE_LETTER_UP = 52,
    JOY_EVENT_ROTATE_LETTER_DOWN = 53,
    JOY_EVENT_PREV_CHAR = 54,
    JOY_EVENT_NEXT_CHAR = 55,
    JOY_EVENT_CARSEL_CHANGE_MODE = 56,
    JOY_EVENT_CARSEL_UNLOCK = 57,
    JOY_EVENT_CARSEL_ORBIT_LEFT = 58,
    JOY_EVENT_CARSEL_ORBIT_RIGHT = 59,
    JOY_EVENT_CARSEL_ORBIT_UP = 60,
    JOY_EVENT_CARSEL_ORBIT_DOWN = 61,
    JOY_EVENT_CARSEL_ORBIT_IN = 62,
    JOY_EVENT_CARSEL_ORBIT_OUT = 63,
    JOY_EVENT_CARSEL_ORBIT_LEFTRIGHT = 64,
    JOY_EVENT_CARSEL_ORBIT_UPDOWN = 65,
    JOY_EVENT_CARSEL_ORBIT_INOUT = 66,
    JOY_EVENT_DIRECTION = 67,
    JOY_EVENT_CHANGE_DEBUG_CAMERA = 68,
    JOY_EVENT_DEBUG_CAMERA_ROTATE_RIGHT = 69,
    JOY_EVENT_DEBUG_CAMERA_ROTATE_LEFT = 70,
    JOY_EVENT_DEBUG_CAMERA_ROTATE_UP = 71,
    JOY_EVENT_DEBUG_CAMERA_ROTATE_DOWN = 72,
    JOY_EVENT_DEBUG_CAMERA_MOVE_CLOSER = 73,
    JOY_EVENT_DEBUG_CAMERA_MOVE_FARTHER = 74,
    JOY_EVENT_DEBUG_CAMERA_NEXT_CAR = 75,
    JOY_EVENT_DEBUG_CAMERA_PREV_CAR = 76,
    JOY_EVENT_DEBUG_CAMERA_FOLLOW_CHOPPER = 77,
    JOY_EVENT_DEBUG_CAMERA_TURN_LEFT = 78,
    JOY_EVENT_DEBUG_CAMERA_TURN_RIGHT = 79,
    JOY_EVENT_DEBUG_CAMERA_TURN_DOWN = 80,
    JOY_EVENT_DEBUG_CAMERA_TURN_UP = 81,
    JOY_EVENT_DEBUG_CAMERA_MOVE_DOWN = 82,
    JOY_EVENT_DEBUG_CAMERA_MOVE_UP = 83,
    JOY_EVENT_DEBUG_CAMERA_MOVE_FWD = 84,
    JOY_EVENT_DEBUG_CAMERA_MOVE_BACK = 85,
    JOY_EVENT_DEBUG_CAMERA_MOVE_LEFT = 86,
    JOY_EVENT_DEBUG_CAMERA_MOVE_RIGHT = 87,
    JOY_EVENT_DEBUG_CAMERA_TURBO = 88,
    JOY_EVENT_DEBUG_CAMERA_SUPER_TURBO = 89,
    JOY_EVENT_DEBUG_CAMERA_SLOW = 90,
    JOY_EVENT_DEBUG_CAMERA_FORWARD_BLINK = 91,
    JOY_EVENT_DEBUG_CAMERA_BACKWARD_BLINK = 92,
    JOY_EVENT_DEBUG_CAMERA_DROP_CAR = 93,
    JOY_EVENT_DEBUG_CAMERA_ROTATE_LEFTRIGHT = 94,
    JOY_EVENT_DEBUG_CAMERA_ROTATE_UPDOWN = 95,
    JOY_EVENT_DEBUG_CAMERA_STRAFE_LEFTRIGHT = 96,
    JOY_EVENT_DEBUG_CAMERA_INOUT = 97,
    JOY_EVENT_TRACKROUTE_EDITOR_TURN_LEFTRIGHT = 98,
    JOY_EVENT_TRACKROUTE_EDITOR_TURN_UPDOWN = 99,
    JOY_EVENT_TRACKROUTE_EDITOR_DRIVE_FORWARDBACKWARD = 100,
    JOY_EVENT_TRACKROUTE_EDITOR_DRIVE_LEFTRIGHT = 101,
    JOY_EVENT_TRACKROUTE_EDITOR_DRIVE_TURBO = 102,
    JOY_EVENT_TRACKROUTE_EDITOR_MOVE_UP = 103,
    JOY_EVENT_TRACKROUTE_EDITOR_MOVE_DOWN = 104,
    JOY_EVENT_TRACKROUTE_EDITOR_DPAD_UP = 105,
    JOY_EVENT_TRACKROUTE_EDITOR_DPAD_DOWN = 106,
    JOY_EVENT_TRACKROUTE_EDITOR_DPAD_LEFT = 107,
    JOY_EVENT_TRACKROUTE_EDITOR_DPAD_RIGHT = 108,
    JOY_EVENT_TRACKROUTE_EDITOR_NEXT_ROUTE = 109,
    JOY_EVENT_TRACKROUTE_EDITOR_NEXT_POINT = 110,
    JOY_EVENT_TRACKROUTE_EDITOR_PREV_POINT = 111,
    JOY_EVENT_TRACKROUTE_EDITOR_RESET_POS = 112,
    JOY_EVENT_TRACKROUTE_EDITOR_EDIT_RIGHT_WIDTH = 113,
    JOY_EVENT_TRACKROUTE_EDITOR_EDIT_LEFT_WIDTH = 114,
    JOY_EVENT_TRACKROUTE_EDITOR_EDIT_POSITION = 115,
    JOY_EVENT_TOGGLE_TRACK_MARKER = 116,
    JOY_EVENT_CAMERA_POV_CHANGE = 117,
    JOY_EVENT_CAMERA_LOOKBACK = 118,
    JOY_EVENT_ZONE_FREEZE = 119,
    JOY_EVENT_ZONE_PREVIEW = 120,
    JOY_EVENT_ZONE_SLOMO = 121,
    JOY_EVENT_ZONE_WARP = 122,
    JOY_EVENT_REPLAY_SKIP = 123,
    JOY_EVENT_REPLAY_PAUSE = 124,
    JOY_EVENT_REPLAY_PREVIOUS_MARKER = 125,
    JOY_EVENT_REPLAY_NEXT_MARKER = 126,
    JOY_EVENT_REPLAY_PLAYBACK_SPEED = 127,
    JOY_EVENT_REPLAY_CAMERA_ANGLE = 128,
    JOY_EVENT_REPLAY_CAMERA_TARGET = 129,
    JOY_EVENT_REPLAY_HELP = 130,
    JOY_EVENT_REPLAY_HIDE_TIMELINE = 131,
    JOY_EVENT_REPLAY_MARK_DEMO = 132,
    JOY_EVENT_REPLAY_TEST_DEMO = 133,
    JOY_EVENT_REPLAY_ERASE_DEMO = 134,
    JOY_EVENT_STEER = 135,
    JOY_EVENT_STEER_ANALOG = 136,
    JOY_EVENT_THROTTLE = 137,
    JOY_EVENT_THROTTLE_ANALOG = 138,
    JOY_EVENT_THROTTLE_ANALOG_ALTERNATE = 139,
    JOY_EVENT_BRAKE = 140,
    JOY_EVENT_BRAKE_ANALOG = 141,
    JOY_EVENT_BRAKE_ANALOG_ALTERNATE = 142,
    JOY_EVENT_REVERSE = 143,
    JOY_EVENT_REVERSE_ANALOG = 144,
    JOY_EVENT_EBRAKE = 145,
    JOY_EVENT_EBRAKE_ANALOG = 146,
    JOY_EVENT_SHIFTUP = 147,
    JOY_EVENT_SHIFTDOWN = 148,
    JOY_EVENT_SHIFTUP_ALTERNATE = 149,
    JOY_EVENT_SHIFTDOWN_ALTERNATE = 150,
    JOY_EVENT_DISENGAGE_CLUTCH = 151,
    JOY_EVENT_DISENGAGE_CLUTCH_ANALOG = 152,
    JOY_EVENT_RESET_CAR = 153,
    JOY_EVENT_HONK_HORN = 154,
    JOY_EVENT_NITRO_BOOST = 155,
    JOY_EVENT_NITRO_BOOST_ALTERNATE = 156,
    JOY_EVENT_REQUEST_ROADBLOCK = 157,
    JOY_EVENT_REQUEST_HELICOPTER = 158,
    JOY_EVENT_FIRE_ROCKET = 159,
    JOY_EVENT_AIM_ROCKET_LR = 160,
    JOY_EVENT_AIM_ROCKET_UD = 161,
    JOY_EVENT_AIM_ZOOM = 162,
    JOY_EVENT_AUTO_PILOT = 163,
    JOY_EVENT_STEER_ROCKET_ANALOG = 164,
    JOY_EVENT_DETONANTE_ROCKET = 165,
    JOY_EVENT_DRAG_RACE_CHANGE_LANE_LEFT = 166,
    JOY_EVENT_DRAG_RACE_CHANGE_LANE_RIGHT = 167,
    JOY_EVENT_DRAG_RACE_CHANGE_LANE_LEFT_ANALOG = 168,
    JOY_EVENT_DRAG_RACE_CHANGE_LANE_RIGHT_ANALOG = 169,
    JOY_EVENT_CYCLE_HUD = 170,
    JOY_EVENT_PAUSE = 171,
    JOY_EVENT_SKIP = 172,
    JOY_EVENT_EASTER_EGG_L1 = 173,
    JOY_EVENT_EASTER_EGG_L2 = 174,
    JOY_EVENT_EASTER_EGG_R1 = 175,
    JOY_EVENT_EASTER_EGG_R2 = 176,
    JOY_EVENT_EASTER_EGG_DPAD_LEFT = 177,
    JOY_EVENT_EASTER_EGG_DPAD_RIGHT = 178,
    JOY_EVENT_EASTER_EGG_SQUARE = 179,
    JOY_EVENT_EASTER_EGG_TRIANGLE = 180,
    JOY_EVENT_EASTER_EGG_L3 = 181,
    JOY_EVENT_EASTER_EGG_R3 = 182,
    JOY_EVENT_EASTER_EGG_DPADU = 183,
    JOY_EVENT_EASTER_EGG_DPADD = 184,
    JOY_EVENT_EASTER_EGG_START = 185,
    JOY_EVENT_EASTER_EGG_SELECT = 186,
    JOY_EVENT_EASTER_EGG_CROSS = 187,
    JOY_EVENT_EASTER_EGG_CIRCLE = 188,
    NUM_JOY_EVENTS = 189,
};
typedef enum JoyEvent JoyEvent;
enum JoylogChannel {
    JOYLOG_CHANNEL_NONE = 0,
    JOYLOG_CHANNEL_CONFIG = 1,
    JOYLOG_CHANNEL_JOYEVENTS = 2,
    JOYLOG_CHANNEL_TIMESTEP = 3,
    JOYLOG_CHANNEL_CHECKSUM = 4,
    JOYLOG_CHANNEL_LOOP_COUNTER = 5,
    JOYLOG_CHANNEL_QUEUEDFILE_STATUS = 6,
    JOYLOG_CHANNEL_MOVIE_PLAYER_STATUS = 7,
    JOYLOG_CHANNEL_MEMORY_CARD = 8,
    JOYLOG_CHANNEL_TRACK_STREAMER = 9,
    JOYLOG_CHANNEL_RANDOM = 10,
    JOYLOG_NUM_CHANNELS = 11,
};
typedef enum JoylogChannel JoylogChannel;
enum JoystickPort {
    JOYSTICK_PORT_NONE = -1,
    JOYSTICK_PORT1 = 0,
    JOYSTICK_PORT2 = 1,
    JOYSTICK_PORT3 = 2,
    JOYSTICK_PORT4 = 3,
};
typedef enum JoystickPort JoystickPort;
enum LOCALESTATE {
    LOCALE_LANGUAGEID = 0,
    LOCALE_LANGUAGECOUNT = 1,
    LOCALE_STRINGCOUNT = 2,
    LOCALE_HASINDEX = 3,
};
typedef enum LOCALESTATE LOCALESTATE;
enum MIPMAP_FLAG {
    MIPMAP_KEEP = -1,
    MIPMAP_OFF = 0,
    MIPMAP_ON = 1,
};
typedef enum MIPMAP_FLAG MIPMAP_FLAG;
enum MemFill_t {
    MEMFILL_NOFILL = 0,
    MEMFILL_FILL = 1,
};
typedef enum MemFill_t MemFill_t;
enum OS_THREAD_STATE {
    OS_THREAD_STATE_READY = 1,
    OS_THREAD_STATE_RUNNING = 2,
    OS_THREAD_STATE_WAITING = 4,
    OS_THREAD_STATE_MORIBUND = 8,
};
typedef enum OS_THREAD_STATE OS_THREAD_STATE;
enum PRINTCHANNEL {
    PRINT_ALL = 0,
    PRINT_REAL = 1,
    PRINT_ALERT = 2,
    PRINT_GENERAL = 3,
    PRINT_MEM = 4,
    PRINT_GRAPHICS = 5,
    PRINT_SHAPE = 6,
    PRINT_FILE = 7,
    PRINT_INPUT = 8,
    PRINT_SYSTEM = 9,
    PRINT_FONT = 10,
    PRINT_CODEC = 11,
    PRINT_MATH = 12,
    PRINT_DEBUG = 13,
    PRINT_USER_CHANNEL = 14,
};
typedef enum PRINTCHANNEL PRINTCHANNEL;
enum PRINTDEVICE {
    PRINT_NULL = 0,
    PRINT_CONSOLE = 1,
    PRINT_DEBUGGER = 2,
    PRINT_LOGFILE = 3,
    PRINT_USER_DEVICE = 4,
};
typedef enum PRINTDEVICE PRINTDEVICE;
enum PeriodicForceNumber {
    PERIODIC_0 = 0,
    PERIODIC_1 = 1,
    PERIODIC_2 = 2,
    PERIODIC_3 = 3,
    PERIODIC_4 = 4,
    PERIODIC_5 = 5,
    PERIODIC_6 = 6,
    PERIODIC_7 = 7,
};
typedef enum PeriodicForceNumber PeriodicForceNumber;
enum QueuedFileStatus {
    WAITING = 0,
    READING = 1,
    QueuedFileStatus_DONE = 2, // im Original: DONE
    ERROR = 3,
};
typedef enum QueuedFileStatus QueuedFileStatus;
enum QuickSplineBasisType {
    QUICKSPLINE_OVERHAUSER = 0,
};
typedef enum QuickSplineBasisType QuickSplineBasisType;
enum QuickSplineEndPointType {
    QUICKSPLINE_LOOP = 0,
    QUICKSPLINE_LINE = 1,
    QUICKSPLINE_EXTRAPOLATED = 2,
};
typedef enum QuickSplineEndPointType QuickSplineEndPointType;
enum RCMessage {
    RC_MSG_NONE = 0,
    RC_ACTIVATE = 1,
    RC_UNACTIVATE = 2,
    RC_TICK = 3,
    RC_SKIP = 4,
    RC_REQUEST_PAUSE = 5,
    RC_REQUEST_UNPAUSE = 6,
    RC_REQUEST_AUTO_REPLAY = 7,
    RC_RESTART_RACE = 8,
    RC_NEXT_RACE = 9,
    RC_QUIT_RACE = 10,
    RC_SKIP_REPLAY = 11,
    RC_START_REPLAY = 12,
    RC_TAKE_AUTOSNAPSHOT = 13,
    RC_MOVE_REPLAY = 14,
    RC_REQUEST_OPEN_TUTORIAL = 15,
    RC_REQUEST_CLOSE_TUTORIAL = 16,
    NUM_RC_MSGS = 17,
};
typedef enum RCMessage RCMessage;
enum RCState {
    RCSTATE_NONE = 0,
    RCSTATE_LOADING = 1,
    RCSTATE_CAMERA_FLYBY = 2,
    RCSTATE_PRERACE = 3,
    RCSTATE_RACE_IN_PROGRESS = 4,
    RCSTATE_INSTANT_REPLAY = 5,
    RCSTATE_AUTO_REPLAY = 6,
    RCSTATE_PAUSE = 7,
    RCSTATE_POSTRACE = 8,
    RCSTATE_AI_DEMO_MODE = 9,
    RCSTATE_REPLAY_DEMO_MODE = 10,
    RCSTATE_TUTORIAL = 11,
    NUM_RCSTATES = 12,
};
typedef enum RCState RCState;
enum RampForceNumber {
    RAMP_0 = 0,
    RAMP_1 = 1,
    RAMP_2 = 2,
    RAMP_3 = 3,
    RAMP_4 = 4,
    RAMP_5 = 5,
    RAMP_6 = 6,
    RAMP_7 = 7,
};
typedef enum RampForceNumber RampForceNumber;
enum ReplayChannel {
    REPLAY_CHANNEL_JOYEVENTS = 0,
    REPLAY_CHANNEL_TIMESTEP = 1,
    REPLAY_CHANNEL_CHECKSUM = 2,
    REPLAY_CHANNEL_SNAPSHOT_MARKER = 3,
    REPLAY_NUM_CHANNELS = 4,
};
typedef enum ReplayChannel ReplayChannel;
enum Result {
    RESULT_OK = 0,
    RESULT_ERR_MEM = -1,
    RESULT_ERR_ALLOCATE = -2,
    RESULT_ERR_HANDLEEXPIRED = -3,
    RESULT_ERR_UNUSED = -4,
    RESULT_ERR_NOTFOUND = -5,
    RESULT_ERR_UNINITIALIZED = -6,
};
typedef enum Result Result;
enum SHAPE_TYPE_ENUM {
    SSH_P_4 = 1,
    SSH_P_8 = 2,
    SSH_ARGB_1555 = 3,
    SSH_RGB_888 = 4,
    SSH_ARGB_8888 = 5,
    SSH_DXT1 = 96,
    SSH_DXT3 = 97,
    SSH_DXT5 = 98,
    SSH_ARGB_8888_IPU_TILED = 6,
    SSH_CLUT_ARGB_1555 = 32,
    SSH_CLUT_ARGB_8888 = 33,
    GSH_P_4 = 24,
    GSH_P_8 = 25,
    GSH_5A3 = 21,
    GSH_RGB_565 = 20,
    GSH_ARGB_8888 = 22,
    GSH_DXT1 = 30,
    GSH_L_4 = 16,
    GSH_L_8 = 17,
    GSH_AL_44 = 18,
    GSH_AL_88 = 19,
    GSH_YUV411 = 28,
    GSH_CLUT_5A3 = 50,
    GSH_CLUT_RGB_565 = 49,
    GSH_CLUT_AL_88 = 48,
    GSH_CLUT_ARGB_8888 = 51,
    XSH_P_8 = 123,
    XSH_P_4 = 121,
    XSH_P_4H = 122,
    XSH_ARGB_4444 = 109,
    XSH_ARGB_1555 = 126,
    XSH_RGB_565 = 120,
    XSH_RGB_888 = 127,
    XSH_ARGB_8888 = 125,
    XSH_DXT1 = 96,
    XSH_DXT3 = 97,
    XSH_DXT5 = 98,
    XSH_A_8 = 100,
    XSH_AL_88 = 101,
    XSH_YUY2 = 104,
    XSH_YUV411 = 108,
    XSH_LVU_655 = 103,
    XSH_CLUT_ARGB_8888 = 42,
    FSH_P_4 = 121,
    FSH_P_8 = 123,
    FSH_P_4H = 122,
    FSH_ARGB_4444 = 109,
    FSH_ARGB_1555 = 126,
    FSH_RGB_565 = 120,
    FSH_RGB_888 = 127,
    FSH_ARGB_8888 = 125,
    FSH_ARGB_6666 = 102,
    FSH_ARGB_2101010 = 106,
    FSH_DXT1 = 96,
    FSH_DXT3 = 97,
    FSH_DXT5 = 98,
    FSH_A_8 = 100,
    FSH_AL_88 = 101,
    FSH_YUY2 = 104,
    FSH_YUV411 = 108,
    FSH_MADYUV = 99,
    FSH_LVU_655 = 103,
    FSH_CLUT_ARGB_1555 = 45,
    FSH_CLUT_RGB_565 = 41,
    FSH_CLUT_ARGB_8888 = 42,
    FSH_CLUT_RGB_666 = 34,
    FSH_CLUT_RGB_888 = 36,
    FSH_CLUT_AXRGB_88565 = 44,
    FSH_CLUT_AXARGB_881555 = 46,
    ASH_P_4 = 1,
    ASH_P_8 = 2,
    ASH_ARGB_1555 = 3,
    ASH_CLUT_ARGB_1555 = 32,
    MSH_P_4 = 88,
    MSH_P_8 = 89,
    MSH_ARGB_1555 = 90,
    MSH_RGB_565 = 91,
    MSH_ARGB_4444 = 92,
    MSH_RGB_888 = 93,
    MSH_ARGB_8888 = 94,
    MSH_CLUT_ARGB_8888 = 56,
    SHP_NAME = 112,
    SHP_HOT_SPOTS = 124,
    SHP_CLUT_ANIMATION = 110,
    SHP_COMMENT = 111,
    SHP_METAL = 105,
};
typedef enum SHAPE_TYPE_ENUM SHAPE_TYPE_ENUM;
enum SHIFTBits {
    s_LEFT = 0,
    s_RIGHT = 1,
    s_DOWN = 2,
    s_UP = 3,
    s_TZ = 4,
    s_TR = 5,
    s_TL = 6,
    s_A = 8,
    s_B = 9,
    s_X = 10,
    s_Y = 11,
    s_MENU = 12,
    s_START = 12,
};
typedef enum SHIFTBits SHIFTBits;
enum STATE {
    ST_NORMAL = 0,
    ST_PERCENT = 1,
    ST_FLAG = 2,
    ST_WIDTH = 3,
    ST_DOT = 4,
    ST_PRECIS = 5,
    ST_SIZE = 6,
    ST_TYPE = 7,
};
typedef enum STATE STATE;
enum STREAMREQUESTSTATE {
    STREAMREQUEST_FREE = 0,
    STREAMREQUEST_PENDING = 1,
    STREAMREQUEST_EXECUTING = 2,
    STREAMREQUEST_COMPLETED = 3,
    STREAMREQUEST_CANCELED = 4,
};
typedef enum STREAMREQUESTSTATE STREAMREQUESTSTATE;
enum STREAMSTATE {
    STREAM_IDLE = 0,
    STREAM_RUNNING = 1,
    STREAM_STOPPED = 2,
};
typedef enum STREAMSTATE STREAMSTATE;
enum SWIZZLE_FLAG {
    SWIZZLE_KEEP = -1,
    SWIZZLE_OFF = 0,
    SWIZZLE_ON = 1,
};
typedef enum SWIZZLE_FLAG SWIZZLE_FLAG;
enum SlotPoolFlags {
    SLOTPOOL_FLAG_OVERFLOW_IF_FULL = 1,
    SLOTPOOL_FLAG_ZERO_ALLOCATED_MEMORY = 2,
    SLOTPOOL_FLAG_WARN_IF_OVERFLOW = 4,
    SLOTPOOL_FLAG_WARN_IF_NONEMPTY_DELETE = 8,
};
typedef enum SlotPoolFlags SlotPoolFlags;
enum SplineType {
    OVERHAUSER_LINE = 0,
    OVERHAUSER_LOOP = 1,
    OVERHAUSER_EXTRAPOLATED = 2,
};
typedef enum SplineType SplineType;
enum TARGET_ID {
    TARGET_FLAYER = 0,
    TARGET_PLAYER1 = 1,
    TARGET_PLAYER2 = 2,
    TARGET_HEADLIGHT_P1 = 3,
    TARGET_HEADLIGHT_P2 = 4,
    TARGET_PLAYER1_RVM = 5,
    TARGET_PLAYER1_REFLECTION = 6,
    TARGET_PLAYER2_REFLECTION = 7,
    TARGET_ENVMAP0F = 8,
    TARGET_ENVMAP0R = 9,
    TARGET_ENVMAP0B = 10,
    TARGET_ENVMAP0L = 11,
    TARGET_ENVMAP0U = 12,
    TARGET_ENVMAP0D = 13,
    TARGET_ENVMAP0_FULL = 14,
    TOTAL_RENDER_TARGETS = 15,
    FIRST_RENDER_TARGET = 0,
    LAST_RENDER_TARGET = 14,
    NUM_RENDER_TARGETS = 15,
};
typedef enum TARGET_ID TARGET_ID;
enum THREAD_PRIORITY {
    IDLE_PRIORITY = -3,
    LOW_PRIORITY = -2,
    BELOW_PRIORITY = -1,
    NORM_PRIORITY = 0,
    ABOVE_PRIORITY = 1,
    HIGH_PRIORITY = 2,
    CRIT_PRIORITY = 3,
};
typedef enum THREAD_PRIORITY THREAD_PRIORITY;
enum TerrainEffect {
    TERRAINFX_DRIVE_OVER = 0,
    TERRAINFX_DRIVE_OVER2 = 1,
    TERRAINFX_SKID_SMOKE = 2,
    TERRAINFX_TIRE_SPEW = 3,
    TERRAINFX_BOTTOM_OUT = 4,
    TERRAINFX_COLLISION_IMPACT = 5,
    TERRAINFX_COLLISION_SCRAPE = 6,
    NUM_TERRAINFX = 7,
};
typedef enum TerrainEffect TerrainEffect;
enum TerrainType {
    TERRAIN_TYPE_NONE = 0,
    TERRAIN_TYPE_ROAD = 1,
    TERRAIN_TYPE_ROAD_WET = 2,
    TERRAIN_TYPE_ROAD_DRIFT = 3,
    TERRAIN_TYPE_ROAD_SMOKE_1 = 4,
    TERRAIN_TYPE_ROAD_SMOKE_2 = 5,
    TERRAIN_TYPE_ROAD_SMOKE_3 = 6,
    TERRAIN_TYPE_BRIDGE = 7,
    TERRAIN_TYPE_DIRT = 8,
    TERRAIN_TYPE_GRAVEL = 9,
    TERRAIN_TYPE_ROUGH_ROAD = 10,
    TERRAIN_TYPE_COBBLESTONE = 11,
    TERRAIN_TYPE_STAIRS = 12,
    TERRAIN_TYPE_PUDDLE = 13,
    TERRAIN_TYPE_DEEP_WATER = 14,
    TERRAIN_TYPE_GRASS = 15,
    TERRAIN_TYPE_SIDEWALK = 16,
    TERRAIN_TYPE_WOOD = 17,
    TERRAIN_TYPE_PLASTIC = 18,
    TERRAIN_TYPE_GLASS = 19,
    TERRAIN_TYPE_SOLID_WALL = 20,
    TERRAIN_TYPE_SEE_THROUGH_WALL = 21,
    TERRAIN_TYPE_PLANT = 22,
    TERRAIN_TYPE_POST = 23,
    TERRAIN_TYPE_PILLAR = 24,
    TERRAIN_TYPE_METAL_GRATE = 25,
    TERRAIN_TYPE_METAL = 26,
    TERRAIN_TYPE_CHAINLINK = 27,
    TERRAIN_TYPE_CAR = 28,
    NUM_TERRAIN_TYPES = 29,
};
typedef enum TerrainType TerrainType;
enum TextureAlphaBlendType {
    TEXBLEND_SRCCOPY = 0,
    TEXBLEND_BLEND = 1,
    TEXBLEND_ADDATIVE = 2,
    TEXBLEND_SUBTRACTIVE = 3,
    TEXBLEND_OVERBRIGHT = 4,
    TEXBLEND_DEST_BLEND = 5,
    TEXBLEND_DEST_ADDATIVE = 6,
    TEXBLEND_DEST_SUBTRACTIVE = 7,
    TEXBLEND_DEST_OVERBRIGHT = 8,
};
typedef enum TextureAlphaBlendType TextureAlphaBlendType;
enum TextureAlphaUsageType {
    TEXUSAGE_NONE = 0,
    TEXUSAGE_PUNCHTHRU = 1,
    TEXUSAGE_MODULATED = 2,
};
typedef enum TextureAlphaUsageType TextureAlphaUsageType;
enum TextureCompressionType {
    TEXCOMP_DEFAULT = 0,
    TEXCOMP_4BIT = 4,
    TEXCOMP_8BIT = 8,
    TEXCOMP_16BIT = 16,
    TEXCOMP_24BIT = 24,
    TEXCOMP_32BIT = 32,
    TEXCOMP_DXT = 33,
    TEXCOMP_S3TC = 34,
    TEXCOMP_DXTC1 = 34,
    TEXCOMP_DXTC3 = 36,
    TEXCOMP_16BIT_1555 = 17,
    TEXCOMP_16BIT_565 = 18,
    TEXCOMP_16BIT_3555 = 19,
    TEXCOMP_8BIT_16 = 128,
    TEXCOMP_8BIT_64 = 129,
};
typedef enum TextureCompressionType TextureCompressionType;
enum TextureLockType {
    TEXLOCK_READ = 0,
    TEXLOCK_WRITE = 1,
    TEXLOCK_READWRITE = 2,
};
typedef enum TextureLockType TextureLockType;
enum TextureMapsIndicies {
    TEXMAP_BASE = 0,
    TEXMAP_MTEX0 = 1,
    TEXMAP_MTEX1 = 2,
    TEXMAP_MTEX2 = 3,
    TEXMAP_NUM = 4,
};
typedef enum TextureMapsIndicies TextureMapsIndicies;
enum TextureScrollType {
    TEXSCROLL_NONE = 0,
    TEXSCROLL_SMOOTH = 1,
    TEXSCROLL_SNAP = 2,
    TEXSCROLL_OFFSETSCALE = 3,
};
typedef enum TextureScrollType TextureScrollType;
enum TopologyTreePolygonFlags {
    TOPOLOGY_TREE_POLYGON_FLAG_OVERLAPPING = 1,
    TOPOLOGY_TREE_POLYGON_FLAG_VERTICAL = 2,
    TOPOLOGY_TREE_POLYGON_FLAG_INFINITELY_HIGH = 4,
    TOPOLOGY_TREE_POLYGON_FLAG_UPSIDE_DOWN = 8,
    TOPOLOGY_TREE_POLYGON_FLAG_4VERTICES = 16,
};
typedef enum TopologyTreePolygonFlags TopologyTreePolygonFlags;
enum Type {
    ANIM_INVALID = -1,
    ANIM_RAWPOSE = 0,
    ANIM_RAWEVENT = 1,
    ANIM_RAWLINEAR = 2,
    ANIM_CYCLE = 3,
    ANIM_EVENTBLENDER = 4,
    ANIM_GRAFT = 5,
    ANIM_POSEBLENDER = 6,
    ANIM_POSEMIRROR = 7,
    ANIM_RUNBLENDER = 8,
    ANIM_TURNBLENDER = 9,
    ANIM_DELTALERP = 10,
    ANIM_DELTAQUAT = 11,
    ANIM_KEYLERP = 12,
    ANIM_KEYQUAT = 13,
    ANIM_PHASE = 14,
    ANIM_COMPOUND = 15,
    ANIM_RAWSTATE = 16,
    ANIM_DELTAQ = 17,
    ANIM_DELTAQFAST = 18,
    ANIM_DELTASINGLEQ = 19,
    ANIM_DELTAF3 = 20,
    ANIM_DELTAF1 = 21,
    ANIM_STATELESSQ = 22,
    ANIM_STATELESSF3 = 23,
    ANIM_MAX = 24,
};
typedef enum Type Type;
enum UnPauseReasonType {
    UNPAUSE_REASON_NONE = 0,
    UNPAUSE_REASON_RESUME_RACE = 1,
    UNPAUSE_REASON_RESTART_RACE = 2,
    UNPAUSE_REASON_QUIT_RACE = 3,
    UNPAUSE_REASON_INSTANT_REPLAY = 4,
};
typedef enum UnPauseReasonType UnPauseReasonType;
enum UnmatchedInterface {
    UNMATCHEDINTERFACE_FAIL = 0,
    UNMATCHEDINTERFACE_ABORT = 1,
};
typedef enum UnmatchedInterface UnmatchedInterface;
enum VIDEO_MODE {
    MODE_PAL = 0,
    MODE_NTSC = 1,
    NUM_VIDEO_MODES = 2,
};
typedef enum VIDEO_MODE VIDEO_MODE;
enum VITVMode {
    VI_TVMODE_NTSC_INT = 0,
    VI_TVMODE_NTSC_DS = 1,
    VI_TVMODE_NTSC_PROG = 2,
    VI_TVMODE_PAL_INT = 4,
    VI_TVMODE_PAL_DS = 5,
    VI_TVMODE_EURGB60_INT = 20,
    VI_TVMODE_EURGB60_DS = 21,
    VI_TVMODE_MPAL_INT = 8,
    VI_TVMODE_MPAL_DS = 9,
    VI_TVMODE_DEBUG_INT = 12,
    VI_TVMODE_DEBUG_PAL_INT = 16,
    VI_TVMODE_DEBUG_PAL_DS = 17,
};
typedef enum VITVMode VITVMode;
enum VIXFBMode {
    VI_XFBMODE_SF = 0,
    VI_XFBMODE_DF = 1,
};
typedef enum VIXFBMode VIXFBMode;
enum VShaderMode {
    eVS_SCREEN = 0,
    eVS_MODEL = 1,
    eVS_PRIM = 2,
    eVS_DEBUG_TEXT = 3,
    eVS_MAXNUM = 4,
};
typedef enum VShaderMode VShaderMode;
enum WeatherType {
    WEATHER_RAIN = 0,
    WEATHER_FOG = 1,
    WEATHER_SNOW = 2,
    WEATHER_WIND = 3,
    WEATHER_FOG_VOLUME = 4,
    NUM_WEATHER_TYPES = 5,
};
typedef enum WeatherType WeatherType;
enum _GXAlphaOp {
    GX_AOP_AND = 0,
    GX_AOP_OR = 1,
    GX_AOP_XOR = 2,
    GX_AOP_XNOR = 3,
    GX_MAX_ALPHAOP = 4,
};
typedef enum _GXAlphaOp _GXAlphaOp;
enum _GXAlphaReadMode {
    GX_READ_00 = 0,
    GX_READ_FF = 1,
    GX_READ_NONE = 2,
};
typedef enum _GXAlphaReadMode _GXAlphaReadMode;
enum _GXAnisotropy {
    GX_ANISO_1 = 0,
    GX_ANISO_2 = 1,
    GX_ANISO_4 = 2,
    GX_MAX_ANISOTROPY = 3,
};
typedef enum _GXAnisotropy _GXAnisotropy;
enum _GXAttnFn {
    GX_AF_SPEC = 0,
    GX_AF_SPOT = 1,
    GX_AF_NONE = 2,
};
typedef enum _GXAttnFn _GXAttnFn;
enum _GXAttr {
    GX_VA_PNMTXIDX = 0,
    GX_VA_TEX0MTXIDX = 1,
    GX_VA_TEX1MTXIDX = 2,
    GX_VA_TEX2MTXIDX = 3,
    GX_VA_TEX3MTXIDX = 4,
    GX_VA_TEX4MTXIDX = 5,
    GX_VA_TEX5MTXIDX = 6,
    GX_VA_TEX6MTXIDX = 7,
    GX_VA_TEX7MTXIDX = 8,
    GX_VA_POS = 9,
    GX_VA_NRM = 10,
    GX_VA_CLR0 = 11,
    GX_VA_CLR1 = 12,
    GX_VA_TEX0 = 13,
    GX_VA_TEX1 = 14,
    GX_VA_TEX2 = 15,
    GX_VA_TEX3 = 16,
    GX_VA_TEX4 = 17,
    GX_VA_TEX5 = 18,
    GX_VA_TEX6 = 19,
    GX_VA_TEX7 = 20,
    GX_POS_MTX_ARRAY = 21,
    GX_NRM_MTX_ARRAY = 22,
    GX_TEX_MTX_ARRAY = 23,
    GX_LIGHT_ARRAY = 24,
    GX_VA_NBT = 25,
    GX_VA_MAX_ATTR = 26,
    GX_VA_NULL = 255,
};
typedef enum _GXAttr _GXAttr;
enum _GXAttrType {
    GX_NONE = 0,
    GX_DIRECT = 1,
    GX_INDEX8 = 2,
    GX_INDEX16 = 3,
};
typedef enum _GXAttrType _GXAttrType;
enum _GXBlendFactor {
    GX_BL_ZERO = 0,
    GX_BL_ONE = 1,
    GX_BL_SRCCLR = 2,
    GX_BL_INVSRCCLR = 3,
    GX_BL_SRCALPHA = 4,
    GX_BL_INVSRCALPHA = 5,
    GX_BL_DSTALPHA = 6,
    GX_BL_INVDSTALPHA = 7,
    GX_BL_DSTCLR = 2,
    GX_BL_INVDSTCLR = 3,
};
typedef enum _GXBlendFactor _GXBlendFactor;
enum _GXBlendMode {
    GX_BM_NONE = 0,
    GX_BM_BLEND = 1,
    GX_BM_LOGIC = 2,
    GX_BM_SUBTRACT = 3,
    GX_MAX_BLENDMODE = 4,
};
typedef enum _GXBlendMode _GXBlendMode;
enum _GXCITexFmt {
    GX_TF_C4 = 8,
    GX_TF_C8 = 9,
    GX_TF_C14X2 = 10,
};
typedef enum _GXCITexFmt _GXCITexFmt;
enum _GXChannelID {
    GX_COLOR0 = 0,
    GX_COLOR1 = 1,
    GX_ALPHA0 = 2,
    GX_ALPHA1 = 3,
    GX_COLOR0A0 = 4,
    GX_COLOR1A1 = 5,
    GX_COLOR_ZERO = 6,
    GX_ALPHA_BUMP = 7,
    GX_ALPHA_BUMPN = 8,
    GX_COLOR_NULL = 255,
};
typedef enum _GXChannelID _GXChannelID;
enum _GXClipMode {
    GX_CLIP_ENABLE = 0,
    GX_CLIP_DISABLE = 1,
};
typedef enum _GXClipMode _GXClipMode;
enum _GXColorSrc {
    GX_SRC_REG = 0,
    GX_SRC_VTX = 1,
};
typedef enum _GXColorSrc _GXColorSrc;
enum _GXCompCnt {
    GX_POS_XY = 0,
    GX_POS_XYZ = 1,
    GX_NRM_XYZ = 0,
    GX_NRM_NBT = 1,
    GX_NRM_NBT3 = 2,
    GX_CLR_RGB = 0,
    GX_CLR_RGBA = 1,
    GX_TEX_S = 0,
    GX_TEX_ST = 1,
};
typedef enum _GXCompCnt _GXCompCnt;
enum _GXCompType {
    GX_U8 = 0,
    GX_S8 = 1,
    GX_U16 = 2,
    GX_S16 = 3,
    GX_F32 = 4,
    GX_RGB565 = 0,
    GX_RGB8 = 1,
    GX_RGBX8 = 2,
    GX_RGBA4 = 3,
    GX_RGBA6 = 4,
    GX_RGBA8 = 5,
};
typedef enum _GXCompType _GXCompType;
enum _GXCompare {
    GX_NEVER = 0,
    GX_LESS = 1,
    GX_EQUAL = 2,
    GX_LEQUAL = 3,
    GX_GREATER = 4,
    GX_NEQUAL = 5,
    GX_GEQUAL = 6,
    GX_ALWAYS = 7,
};
typedef enum _GXCompare _GXCompare;
enum _GXCopyMode {
    GX_COPY_PROGRESSIVE = 0,
    GX_COPY_INTLC_EVEN = 2,
    GX_COPY_INTLC_ODD = 3,
};
typedef enum _GXCopyMode _GXCopyMode;
enum _GXCullMode {
    GX_CULL_NONE = 0,
    GX_CULL_FRONT = 1,
    GX_CULL_BACK = 2,
    GX_CULL_ALL = 3,
};
typedef enum _GXCullMode _GXCullMode;
enum _GXDiffuseFn {
    GX_DF_NONE = 0,
    GX_DF_SIGN = 1,
    GX_DF_CLAMP = 2,
};
typedef enum _GXDiffuseFn _GXDiffuseFn;
enum _GXDistAttnFn {
    GX_DA_OFF = 0,
    GX_DA_GENTLE = 1,
    GX_DA_MEDIUM = 2,
    GX_DA_STEEP = 3,
};
typedef enum _GXDistAttnFn _GXDistAttnFn;
enum _GXEvent {
    GX_VCACHE_MISS_ALL = 0,
    GX_VCACHE_MISS_POS = 1,
    GX_VCACHE_MISS_NRM = 2,
};
typedef enum _GXEvent _GXEvent;
enum _GXFBClamp {
    GX_CLAMP_NONE = 0,
    GX_CLAMP_TOP = 1,
    GX_CLAMP_BOTTOM = 2,
};
typedef enum _GXFBClamp _GXFBClamp;
enum _GXFogType {
    GX_FOG_NONE = 0,
    GX_FOG_PERSP_LIN = 2,
    GX_FOG_PERSP_EXP = 4,
    GX_FOG_PERSP_EXP2 = 5,
    GX_FOG_PERSP_REVEXP = 6,
    GX_FOG_PERSP_REVEXP2 = 7,
    GX_FOG_ORTHO_LIN = 10,
    GX_FOG_ORTHO_EXP = 12,
    GX_FOG_ORTHO_EXP2 = 13,
    GX_FOG_ORTHO_REVEXP = 14,
    GX_FOG_ORTHO_REVEXP2 = 15,
    GX_FOG_LIN = 2,
    GX_FOG_EXP = 4,
    GX_FOG_EXP2 = 5,
    GX_FOG_REVEXP = 6,
    GX_FOG_REVEXP2 = 7,
};
typedef enum _GXFogType _GXFogType;
enum _GXGamma {
    GX_GM_1_0 = 0,
    GX_GM_1_7 = 1,
    GX_GM_2_2 = 2,
};
typedef enum _GXGamma _GXGamma;
enum _GXIndTexAlphaSel {
    GX_ITBA_OFF = 0,
    GX_ITBA_S = 1,
    GX_ITBA_T = 2,
    GX_ITBA_U = 3,
    GX_MAX_ITBALPHA = 4,
};
typedef enum _GXIndTexAlphaSel _GXIndTexAlphaSel;
enum _GXIndTexBiasSel {
    GX_ITB_NONE = 0,
    GX_ITB_S = 1,
    GX_ITB_T = 2,
    GX_ITB_ST = 3,
    GX_ITB_U = 4,
    GX_ITB_SU = 5,
    GX_ITB_TU = 6,
    GX_ITB_STU = 7,
    GX_MAX_ITBIAS = 8,
};
typedef enum _GXIndTexBiasSel _GXIndTexBiasSel;
enum _GXIndTexFormat {
    GX_ITF_8 = 0,
    GX_ITF_5 = 1,
    GX_ITF_4 = 2,
    GX_ITF_3 = 3,
    GX_MAX_ITFORMAT = 4,
};
typedef enum _GXIndTexFormat _GXIndTexFormat;
enum _GXIndTexMtxID {
    GX_ITM_OFF = 0,
    GX_ITM_0 = 1,
    GX_ITM_1 = 2,
    GX_ITM_2 = 3,
    GX_ITM_S0 = 5,
    GX_ITM_S1 = 6,
    GX_ITM_S2 = 7,
    GX_ITM_T0 = 9,
    GX_ITM_T1 = 10,
    GX_ITM_T2 = 11,
};
typedef enum _GXIndTexMtxID _GXIndTexMtxID;
enum _GXIndTexScale {
    GX_ITS_1 = 0,
    GX_ITS_2 = 1,
    GX_ITS_4 = 2,
    GX_ITS_8 = 3,
    GX_ITS_16 = 4,
    GX_ITS_32 = 5,
    GX_ITS_64 = 6,
    GX_ITS_128 = 7,
    GX_ITS_256 = 8,
    GX_MAX_ITSCALE = 9,
};
typedef enum _GXIndTexScale _GXIndTexScale;
enum _GXIndTexStageID {
    GX_INDTEXSTAGE0 = 0,
    GX_INDTEXSTAGE1 = 1,
    GX_INDTEXSTAGE2 = 2,
    GX_INDTEXSTAGE3 = 3,
    GX_MAX_INDTEXSTAGE = 4,
};
typedef enum _GXIndTexStageID _GXIndTexStageID;
enum _GXIndTexWrap {
    GX_ITW_OFF = 0,
    GX_ITW_256 = 1,
    GX_ITW_128 = 2,
    GX_ITW_64 = 3,
    GX_ITW_32 = 4,
    GX_ITW_16 = 5,
    GX_ITW_0 = 6,
    GX_MAX_ITWRAP = 7,
};
typedef enum _GXIndTexWrap _GXIndTexWrap;
enum _GXLightID {
    GX_LIGHT0 = 1,
    GX_LIGHT1 = 2,
    GX_LIGHT2 = 4,
    GX_LIGHT3 = 8,
    GX_LIGHT4 = 16,
    GX_LIGHT5 = 32,
    GX_LIGHT6 = 64,
    GX_LIGHT7 = 128,
    GX_MAX_LIGHT = 256,
    GX_LIGHT_NULL = 0,
};
typedef enum _GXLightID _GXLightID;
enum _GXLogicOp {
    GX_LO_CLEAR = 0,
    GX_LO_AND = 1,
    GX_LO_REVAND = 2,
    GX_LO_COPY = 3,
    GX_LO_INVAND = 4,
    GX_LO_NOOP = 5,
    GX_LO_XOR = 6,
    GX_LO_OR = 7,
    GX_LO_NOR = 8,
    GX_LO_EQUIV = 9,
    GX_LO_INV = 10,
    GX_LO_REVOR = 11,
    GX_LO_INVCOPY = 12,
    GX_LO_INVOR = 13,
    GX_LO_NAND = 14,
    GX_LO_SET = 15,
};
typedef enum _GXLogicOp _GXLogicOp;
enum _GXMiscToken {
    GX_MT_XF_FLUSH = 1,
    GX_MT_DL_SAVE_CONTEXT = 2,
    GX_MT_ABORT_WAIT_COPYOUT = 3,
    GX_MT_NULL = 0,
};
typedef enum _GXMiscToken _GXMiscToken;
enum _GXPTTexMtx {
    GX_PTTEXMTX0 = 64,
    GX_PTTEXMTX1 = 67,
    GX_PTTEXMTX2 = 70,
    GX_PTTEXMTX3 = 73,
    GX_PTTEXMTX4 = 76,
    GX_PTTEXMTX5 = 79,
    GX_PTTEXMTX6 = 82,
    GX_PTTEXMTX7 = 85,
    GX_PTTEXMTX8 = 88,
    GX_PTTEXMTX9 = 91,
    GX_PTTEXMTX10 = 94,
    GX_PTTEXMTX11 = 97,
    GX_PTTEXMTX12 = 100,
    GX_PTTEXMTX13 = 103,
    GX_PTTEXMTX14 = 106,
    GX_PTTEXMTX15 = 109,
    GX_PTTEXMTX16 = 112,
    GX_PTTEXMTX17 = 115,
    GX_PTTEXMTX18 = 118,
    GX_PTTEXMTX19 = 121,
    GX_PTIDENTITY = 125,
};
typedef enum _GXPTTexMtx _GXPTTexMtx;
enum _GXPerf0 {
    GX_PERF0_VERTICES = 0,
    GX_PERF0_CLIP_VTX = 1,
    GX_PERF0_CLIP_CLKS = 2,
    GX_PERF0_XF_WAIT_IN = 3,
    GX_PERF0_XF_WAIT_OUT = 4,
    GX_PERF0_XF_XFRM_CLKS = 5,
    GX_PERF0_XF_LIT_CLKS = 6,
    GX_PERF0_XF_BOT_CLKS = 7,
    GX_PERF0_XF_REGLD_CLKS = 8,
    GX_PERF0_XF_REGRD_CLKS = 9,
    GX_PERF0_CLIP_RATIO = 10,
    GX_PERF0_TRIANGLES = 11,
    GX_PERF0_TRIANGLES_CULLED = 12,
    GX_PERF0_TRIANGLES_PASSED = 13,
    GX_PERF0_TRIANGLES_SCISSORED = 14,
    GX_PERF0_TRIANGLES_0TEX = 15,
    GX_PERF0_TRIANGLES_1TEX = 16,
    GX_PERF0_TRIANGLES_2TEX = 17,
    GX_PERF0_TRIANGLES_3TEX = 18,
    GX_PERF0_TRIANGLES_4TEX = 19,
    GX_PERF0_TRIANGLES_5TEX = 20,
    GX_PERF0_TRIANGLES_6TEX = 21,
    GX_PERF0_TRIANGLES_7TEX = 22,
    GX_PERF0_TRIANGLES_8TEX = 23,
    GX_PERF0_TRIANGLES_0CLR = 24,
    GX_PERF0_TRIANGLES_1CLR = 25,
    GX_PERF0_TRIANGLES_2CLR = 26,
    GX_PERF0_QUAD_0CVG = 27,
    GX_PERF0_QUAD_NON0CVG = 28,
    GX_PERF0_QUAD_1CVG = 29,
    GX_PERF0_QUAD_2CVG = 30,
    GX_PERF0_QUAD_3CVG = 31,
    GX_PERF0_QUAD_4CVG = 32,
    GX_PERF0_AVG_QUAD_CNT = 33,
    GX_PERF0_CLOCKS = 34,
    GX_PERF0_NONE = 35,
};
typedef enum _GXPerf0 _GXPerf0;
enum _GXPerf1 {
    GX_PERF1_TEXELS = 0,
    GX_PERF1_TX_IDLE = 1,
    GX_PERF1_TX_REGS = 2,
    GX_PERF1_TX_MEMSTALL = 3,
    GX_PERF1_TC_CHECK1_2 = 4,
    GX_PERF1_TC_CHECK3_4 = 5,
    GX_PERF1_TC_CHECK5_6 = 6,
    GX_PERF1_TC_CHECK7_8 = 7,
    GX_PERF1_TC_MISS = 8,
    GX_PERF1_VC_ELEMQ_FULL = 9,
    GX_PERF1_VC_MISSQ_FULL = 10,
    GX_PERF1_VC_MEMREQ_FULL = 11,
    GX_PERF1_VC_STATUS7 = 12,
    GX_PERF1_VC_MISSREP_FULL = 13,
    GX_PERF1_VC_STREAMBUF_LOW = 14,
    GX_PERF1_VC_ALL_STALLS = 15,
    GX_PERF1_VERTICES = 16,
    GX_PERF1_FIFO_REQ = 17,
    GX_PERF1_CALL_REQ = 18,
    GX_PERF1_VC_MISS_REQ = 19,
    GX_PERF1_CP_ALL_REQ = 20,
    GX_PERF1_CLOCKS = 21,
    GX_PERF1_NONE = 22,
};
typedef enum _GXPerf1 _GXPerf1;
enum _GXPixelFmt {
    GX_PF_RGB8_Z24 = 0,
    GX_PF_RGBA6_Z24 = 1,
    GX_PF_RGB565_Z16 = 2,
    GX_PF_Z24 = 3,
    GX_PF_Y8 = 4,
    GX_PF_U8 = 5,
    GX_PF_V8 = 6,
    GX_PF_YUV420 = 7,
};
typedef enum _GXPixelFmt _GXPixelFmt;
enum _GXPosNrmMtx {
    GX_PNMTX0 = 0,
    GX_PNMTX1 = 3,
    GX_PNMTX2 = 6,
    GX_PNMTX3 = 9,
    GX_PNMTX4 = 12,
    GX_PNMTX5 = 15,
    GX_PNMTX6 = 18,
    GX_PNMTX7 = 21,
    GX_PNMTX8 = 24,
    GX_PNMTX9 = 27,
};
typedef enum _GXPosNrmMtx _GXPosNrmMtx;
enum _GXPrimitive {
    GX_POINTS = 184,
    GX_LINES = 168,
    GX_LINESTRIP = 176,
    GX_TRIANGLES = 144,
    GX_TRIANGLESTRIP = 152,
    GX_TRIANGLEFAN = 160,
    GX_QUADS = 128,
};
typedef enum _GXPrimitive _GXPrimitive;
enum _GXProjectionType {
    GX_PERSPECTIVE = 0,
    GX_ORTHOGRAPHIC = 1,
};
typedef enum _GXProjectionType _GXProjectionType;
enum _GXSpotFn {
    GX_SP_OFF = 0,
    GX_SP_FLAT = 1,
    GX_SP_COS = 2,
    GX_SP_COS2 = 3,
    GX_SP_SHARP = 4,
    GX_SP_RING1 = 5,
    GX_SP_RING2 = 6,
};
typedef enum _GXSpotFn _GXSpotFn;
enum _GXTevAlphaArg {
    GX_CA_APREV = 0,
    GX_CA_A0 = 1,
    GX_CA_A1 = 2,
    GX_CA_A2 = 3,
    GX_CA_TEXA = 4,
    GX_CA_RASA = 5,
    GX_CA_KONST = 6,
    GX_CA_ZERO = 7,
    GX_CA_ONE = 6,
};
typedef enum _GXTevAlphaArg _GXTevAlphaArg;
enum _GXTevBias {
    GX_TB_ZERO = 0,
    GX_TB_ADDHALF = 1,
    GX_TB_SUBHALF = 2,
    GX_MAX_TEVBIAS = 3,
};
typedef enum _GXTevBias _GXTevBias;
enum _GXTevClampMode {
    GX_TC_LINEAR = 0,
    GX_TC_GE = 1,
    GX_TC_EQ = 2,
    GX_TC_LE = 3,
    GX_MAX_TEVCLAMPMODE = 4,
};
typedef enum _GXTevClampMode _GXTevClampMode;
enum _GXTevColorArg {
    GX_CC_CPREV = 0,
    GX_CC_APREV = 1,
    GX_CC_C0 = 2,
    GX_CC_A0 = 3,
    GX_CC_C1 = 4,
    GX_CC_A1 = 5,
    GX_CC_C2 = 6,
    GX_CC_A2 = 7,
    GX_CC_TEXC = 8,
    GX_CC_TEXA = 9,
    GX_CC_RASC = 10,
    GX_CC_RASA = 11,
    GX_CC_ONE = 12,
    GX_CC_HALF = 13,
    GX_CC_KONST = 14,
    GX_CC_ZERO = 15,
    GX_CC_TEXRRR = 16,
    GX_CC_TEXGGG = 17,
    GX_CC_TEXBBB = 18,
    GX_CC_QUARTER = 14,
};
typedef enum _GXTevColorArg _GXTevColorArg;
enum _GXTevColorChan {
    GX_CH_RED = 0,
    GX_CH_GREEN = 1,
    GX_CH_BLUE = 2,
    GX_CH_ALPHA = 3,
};
typedef enum _GXTevColorChan _GXTevColorChan;
enum _GXTevKAlphaSel {
    GX_TEV_KASEL_8_8 = 0,
    GX_TEV_KASEL_7_8 = 1,
    GX_TEV_KASEL_6_8 = 2,
    GX_TEV_KASEL_5_8 = 3,
    GX_TEV_KASEL_4_8 = 4,
    GX_TEV_KASEL_3_8 = 5,
    GX_TEV_KASEL_2_8 = 6,
    GX_TEV_KASEL_1_8 = 7,
    GX_TEV_KASEL_1 = 0,
    GX_TEV_KASEL_3_4 = 2,
    GX_TEV_KASEL_1_2 = 4,
    GX_TEV_KASEL_1_4 = 6,
    GX_TEV_KASEL_K0_R = 16,
    GX_TEV_KASEL_K1_R = 17,
    GX_TEV_KASEL_K2_R = 18,
    GX_TEV_KASEL_K3_R = 19,
    GX_TEV_KASEL_K0_G = 20,
    GX_TEV_KASEL_K1_G = 21,
    GX_TEV_KASEL_K2_G = 22,
    GX_TEV_KASEL_K3_G = 23,
    GX_TEV_KASEL_K0_B = 24,
    GX_TEV_KASEL_K1_B = 25,
    GX_TEV_KASEL_K2_B = 26,
    GX_TEV_KASEL_K3_B = 27,
    GX_TEV_KASEL_K0_A = 28,
    GX_TEV_KASEL_K1_A = 29,
    GX_TEV_KASEL_K2_A = 30,
    GX_TEV_KASEL_K3_A = 31,
};
typedef enum _GXTevKAlphaSel _GXTevKAlphaSel;
enum _GXTevKColorID {
    GX_KCOLOR0 = 0,
    GX_KCOLOR1 = 1,
    GX_KCOLOR2 = 2,
    GX_KCOLOR3 = 3,
    GX_MAX_KCOLOR = 4,
};
typedef enum _GXTevKColorID _GXTevKColorID;
enum _GXTevKColorSel {
    GX_TEV_KCSEL_8_8 = 0,
    GX_TEV_KCSEL_7_8 = 1,
    GX_TEV_KCSEL_6_8 = 2,
    GX_TEV_KCSEL_5_8 = 3,
    GX_TEV_KCSEL_4_8 = 4,
    GX_TEV_KCSEL_3_8 = 5,
    GX_TEV_KCSEL_2_8 = 6,
    GX_TEV_KCSEL_1_8 = 7,
    GX_TEV_KCSEL_1 = 0,
    GX_TEV_KCSEL_3_4 = 2,
    GX_TEV_KCSEL_1_2 = 4,
    GX_TEV_KCSEL_1_4 = 6,
    GX_TEV_KCSEL_K0 = 12,
    GX_TEV_KCSEL_K1 = 13,
    GX_TEV_KCSEL_K2 = 14,
    GX_TEV_KCSEL_K3 = 15,
    GX_TEV_KCSEL_K0_R = 16,
    GX_TEV_KCSEL_K1_R = 17,
    GX_TEV_KCSEL_K2_R = 18,
    GX_TEV_KCSEL_K3_R = 19,
    GX_TEV_KCSEL_K0_G = 20,
    GX_TEV_KCSEL_K1_G = 21,
    GX_TEV_KCSEL_K2_G = 22,
    GX_TEV_KCSEL_K3_G = 23,
    GX_TEV_KCSEL_K0_B = 24,
    GX_TEV_KCSEL_K1_B = 25,
    GX_TEV_KCSEL_K2_B = 26,
    GX_TEV_KCSEL_K3_B = 27,
    GX_TEV_KCSEL_K0_A = 28,
    GX_TEV_KCSEL_K1_A = 29,
    GX_TEV_KCSEL_K2_A = 30,
    GX_TEV_KCSEL_K3_A = 31,
};
typedef enum _GXTevKColorSel _GXTevKColorSel;
enum _GXTevMode {
    GX_MODULATE = 0,
    GX_DECAL = 1,
    GX_BLEND = 2,
    GX_REPLACE = 3,
    GX_PASSCLR = 4,
};
typedef enum _GXTevMode _GXTevMode;
enum _GXTevOp {
    GX_TEV_ADD = 0,
    GX_TEV_SUB = 1,
    GX_TEV_COMP_R8_GT = 8,
    GX_TEV_COMP_R8_EQ = 9,
    GX_TEV_COMP_GR16_GT = 10,
    GX_TEV_COMP_GR16_EQ = 11,
    GX_TEV_COMP_BGR24_GT = 12,
    GX_TEV_COMP_BGR24_EQ = 13,
    GX_TEV_COMP_RGB8_GT = 14,
    GX_TEV_COMP_RGB8_EQ = 15,
    GX_TEV_COMP_A8_GT = 14,
    GX_TEV_COMP_A8_EQ = 15,
};
typedef enum _GXTevOp _GXTevOp;
enum _GXTevRegID {
    GX_TEVPREV = 0,
    GX_TEVREG0 = 1,
    GX_TEVREG1 = 2,
    GX_TEVREG2 = 3,
    GX_MAX_TEVREG = 4,
};
typedef enum _GXTevRegID _GXTevRegID;
enum _GXTevScale {
    GX_CS_SCALE_1 = 0,
    GX_CS_SCALE_2 = 1,
    GX_CS_SCALE_4 = 2,
    GX_CS_DIVIDE_2 = 3,
    GX_MAX_TEVSCALE = 4,
};
typedef enum _GXTevScale _GXTevScale;
enum _GXTevStageID {
    GX_TEVSTAGE0 = 0,
    GX_TEVSTAGE1 = 1,
    GX_TEVSTAGE2 = 2,
    GX_TEVSTAGE3 = 3,
    GX_TEVSTAGE4 = 4,
    GX_TEVSTAGE5 = 5,
    GX_TEVSTAGE6 = 6,
    GX_TEVSTAGE7 = 7,
    GX_TEVSTAGE8 = 8,
    GX_TEVSTAGE9 = 9,
    GX_TEVSTAGE10 = 10,
    GX_TEVSTAGE11 = 11,
    GX_TEVSTAGE12 = 12,
    GX_TEVSTAGE13 = 13,
    GX_TEVSTAGE14 = 14,
    GX_TEVSTAGE15 = 15,
    GX_MAX_TEVSTAGE = 16,
};
typedef enum _GXTevStageID _GXTevStageID;
enum _GXTevSwapSel {
    GX_TEV_SWAP0 = 0,
    GX_TEV_SWAP1 = 1,
    GX_TEV_SWAP2 = 2,
    GX_TEV_SWAP3 = 3,
    GX_MAX_TEVSWAP = 4,
};
typedef enum _GXTevSwapSel _GXTevSwapSel;
enum _GXTexCacheSize {
    GX_TEXCACHE_32K = 0,
    GX_TEXCACHE_128K = 1,
    GX_TEXCACHE_512K = 2,
    GX_TEXCACHE_NONE = 3,
};
typedef enum _GXTexCacheSize _GXTexCacheSize;
enum _GXTexCoordID {
    GX_TEXCOORD0 = 0,
    GX_TEXCOORD1 = 1,
    GX_TEXCOORD2 = 2,
    GX_TEXCOORD3 = 3,
    GX_TEXCOORD4 = 4,
    GX_TEXCOORD5 = 5,
    GX_TEXCOORD6 = 6,
    GX_TEXCOORD7 = 7,
    GX_MAX_TEXCOORD = 8,
    GX_TEXCOORD_NULL = 255,
};
typedef enum _GXTexCoordID _GXTexCoordID;
enum _GXTexFilter {
    GX_NEAR = 0,
    GX_LINEAR = 1,
    GX_NEAR_MIP_NEAR = 2,
    GX_LIN_MIP_NEAR = 3,
    GX_NEAR_MIP_LIN = 4,
    GX_LIN_MIP_LIN = 5,
};
typedef enum _GXTexFilter _GXTexFilter;
enum _GXTexFmt {
    GX_TF_I4 = 0,
    GX_TF_I8 = 1,
    GX_TF_IA4 = 2,
    GX_TF_IA8 = 3,
    GX_TF_RGB565 = 4,
    GX_TF_RGB5A3 = 5,
    GX_TF_RGBA8 = 6,
    GX_TF_CMPR = 14,
    GX_CTF_R4 = 32,
    GX_CTF_RA4 = 34,
    GX_CTF_RA8 = 35,
    GX_CTF_YUVA8 = 38,
    GX_CTF_A8 = 39,
    GX_CTF_R8 = 40,
    GX_CTF_G8 = 41,
    GX_CTF_B8 = 42,
    GX_CTF_RG8 = 43,
    GX_CTF_GB8 = 44,
    GX_TF_Z8 = 17,
    GX_TF_Z16 = 19,
    GX_TF_Z24X8 = 22,
    GX_CTF_Z4 = 48,
    GX_CTF_Z8M = 57,
    GX_CTF_Z8L = 58,
    GX_CTF_Z16L = 60,
    GX_TF_A8 = 39,
};
typedef enum _GXTexFmt _GXTexFmt;
enum _GXTexGenSrc {
    GX_TG_POS = 0,
    GX_TG_NRM = 1,
    GX_TG_BINRM = 2,
    GX_TG_TANGENT = 3,
    GX_TG_TEX0 = 4,
    GX_TG_TEX1 = 5,
    GX_TG_TEX2 = 6,
    GX_TG_TEX3 = 7,
    GX_TG_TEX4 = 8,
    GX_TG_TEX5 = 9,
    GX_TG_TEX6 = 10,
    GX_TG_TEX7 = 11,
    GX_TG_TEXCOORD0 = 12,
    GX_TG_TEXCOORD1 = 13,
    GX_TG_TEXCOORD2 = 14,
    GX_TG_TEXCOORD3 = 15,
    GX_TG_TEXCOORD4 = 16,
    GX_TG_TEXCOORD5 = 17,
    GX_TG_TEXCOORD6 = 18,
    GX_TG_COLOR0 = 19,
    GX_TG_COLOR1 = 20,
};
typedef enum _GXTexGenSrc _GXTexGenSrc;
enum _GXTexGenType {
    GX_TG_MTX3x4 = 0,
    GX_TG_MTX2x4 = 1,
    GX_TG_BUMP0 = 2,
    GX_TG_BUMP1 = 3,
    GX_TG_BUMP2 = 4,
    GX_TG_BUMP3 = 5,
    GX_TG_BUMP4 = 6,
    GX_TG_BUMP5 = 7,
    GX_TG_BUMP6 = 8,
    GX_TG_BUMP7 = 9,
    GX_TG_SRTG = 10,
};
typedef enum _GXTexGenType _GXTexGenType;
enum _GXTexMapID {
    GX_TEXMAP0 = 0,
    GX_TEXMAP1 = 1,
    GX_TEXMAP2 = 2,
    GX_TEXMAP3 = 3,
    GX_TEXMAP4 = 4,
    GX_TEXMAP5 = 5,
    GX_TEXMAP6 = 6,
    GX_TEXMAP7 = 7,
    GX_MAX_TEXMAP = 8,
    GX_TEXMAP_NULL = 255,
    GX_TEX_DISABLE = 256,
};
typedef enum _GXTexMapID _GXTexMapID;
enum _GXTexMtx {
    GX_TEXMTX0 = 30,
    GX_TEXMTX1 = 33,
    GX_TEXMTX2 = 36,
    GX_TEXMTX3 = 39,
    GX_TEXMTX4 = 42,
    GX_TEXMTX5 = 45,
    GX_TEXMTX6 = 48,
    GX_TEXMTX7 = 51,
    GX_TEXMTX8 = 54,
    GX_TEXMTX9 = 57,
    GX_IDENTITY = 60,
};
typedef enum _GXTexMtx _GXTexMtx;
enum _GXTexMtxType {
    GX_MTX3x4 = 0,
    GX_MTX2x4 = 1,
};
typedef enum _GXTexMtxType _GXTexMtxType;
enum _GXTexOffset {
    GX_TO_ZERO = 0,
    GX_TO_SIXTEENTH = 1,
    GX_TO_EIGHTH = 2,
    GX_TO_FOURTH = 3,
    GX_TO_HALF = 4,
    GX_TO_ONE = 5,
    GX_MAX_TEXOFFSET = 6,
};
typedef enum _GXTexOffset _GXTexOffset;
enum _GXTexWrapMode {
    GX_CLAMP = 0,
    GX_REPEAT = 1,
    GX_MIRROR = 2,
    GX_MAX_TEXWRAPMODE = 3,
};
typedef enum _GXTexWrapMode _GXTexWrapMode;
enum _GXTlut {
    GX_TLUT0 = 0,
    GX_TLUT1 = 1,
    GX_TLUT2 = 2,
    GX_TLUT3 = 3,
    GX_TLUT4 = 4,
    GX_TLUT5 = 5,
    GX_TLUT6 = 6,
    GX_TLUT7 = 7,
    GX_TLUT8 = 8,
    GX_TLUT9 = 9,
    GX_TLUT10 = 10,
    GX_TLUT11 = 11,
    GX_TLUT12 = 12,
    GX_TLUT13 = 13,
    GX_TLUT14 = 14,
    GX_TLUT15 = 15,
    GX_BIGTLUT0 = 16,
    GX_BIGTLUT1 = 17,
    GX_BIGTLUT2 = 18,
    GX_BIGTLUT3 = 19,
};
typedef enum _GXTlut _GXTlut;
enum _GXTlutFmt {
    GX_TL_IA8 = 0,
    GX_TL_RGB565 = 1,
    GX_TL_RGB5A3 = 2,
    GX_MAX_TLUTFMT = 3,
};
typedef enum _GXTlutFmt _GXTlutFmt;
enum _GXTlutSize {
    GX_TLUT_16 = 1,
    GX_TLUT_32 = 2,
    GX_TLUT_64 = 4,
    GX_TLUT_128 = 8,
    GX_TLUT_256 = 16,
    GX_TLUT_512 = 32,
    GX_TLUT_1K = 64,
    GX_TLUT_2K = 128,
    GX_TLUT_4K = 256,
    GX_TLUT_8K = 512,
    GX_TLUT_16K = 1024,
};
typedef enum _GXTlutSize _GXTlutSize;
enum _GXVCachePerf {
    GX_VC_POS = 0,
    GX_VC_NRM = 1,
    GX_VC_CLR0 = 2,
    GX_VC_CLR1 = 3,
    GX_VC_TEX0 = 4,
    GX_VC_TEX1 = 5,
    GX_VC_TEX2 = 6,
    GX_VC_TEX3 = 7,
    GX_VC_TEX4 = 8,
    GX_VC_TEX5 = 9,
    GX_VC_TEX6 = 10,
    GX_VC_TEX7 = 11,
    GX_VC_ALL = 15,
};
typedef enum _GXVCachePerf _GXVCachePerf;
enum _GXVtxFmt {
    GX_VTXFMT0 = 0,
    GX_VTXFMT1 = 1,
    GX_VTXFMT2 = 2,
    GX_VTXFMT3 = 3,
    GX_VTXFMT4 = 4,
    GX_VTXFMT5 = 5,
    GX_VTXFMT6 = 6,
    GX_VTXFMT7 = 7,
    GX_MAX_VTXFMT = 8,
};
typedef enum _GXVtxFmt _GXVtxFmt;
enum _GXXFFlushVal {
    GX_XF_FLUSH_NONE = 0,
    GX_XF_FLUSH_SAFE = 8,
};
typedef enum _GXXFFlushVal _GXXFFlushVal;
enum _GXZFmt16 {
    GX_ZC_LINEAR = 0,
    GX_ZC_NEAR = 1,
    GX_ZC_MID = 2,
    GX_ZC_FAR = 3,
};
typedef enum _GXZFmt16 _GXZFmt16;
enum _GXZTexOp {
    GX_ZT_DISABLE = 0,
    GX_ZT_ADD = 1,
    GX_ZT_REPLACE = 2,
    GX_MAX_ZTEXOP = 3,
};
typedef enum _GXZTexOp _GXZTexOp;
enum __fdlibm_version {
    __fdlibm_ieee = -1,
    __fdlibm_svid = 0,
    __fdlibm_xopen = 1,
    __fdlibm_posix = 2,
};
typedef enum __fdlibm_version __fdlibm_version;
enum afxEmitterState {
    AFX_STATE_OFF = 0,
    AFX_STATE_ON = 1,
    AFX_STATE_DISABLED = 2,
    AFX_STATE_LIBRARY = 3,
};
typedef enum afxEmitterState afxEmitterState;
enum eBackgroundStyle {
    BACKGROUND_STYLE_PLAIN = 0,
    BACKGROUND_STYLE_PHOTO = 1,
    BACKGROUND_STYLE_ABSTRACT = 2,
};
typedef enum eBackgroundStyle eBackgroundStyle;
enum eBuildRegion {
    BUILD_REGION_AMERICA = 0,
    BUILD_REGION_EUROPE = 1,
    BUILD_REGION_KOREA = 2,
    BUILD_REGION_JAPAN = 3,
    NUM_BUILD_REGIONS = 4,
};
typedef enum eBuildRegion eBuildRegion;
enum eCarPositionMarkerFlags {
    CPM_HEADLIGHT = 0,
    CPM_BRAKELIGHT = 1,
    CPM_REVERSELIGHT = 2,
    CPM_FOGLIGHT = 3,
    CPM_COPLIGHTRED = 4,
    CPM_COPLIGHTBLUE = 5,
    CPM_COPLIGHTWHITE = 6,
    CPM_TYPE_MASK = 255,
    CPM_TYPE_SHIFT = 0,
    CPM_DIR_PX = 0,
    CPM_DIR_MX = 256,
    CPM_DIR_PY = 512,
    CPM_DIR_MY = 768,
    CPM_DIR_PMX = 1024,
    CPM_DIR_PMY = 1280,
    CPM_DIR_MASK = 65280,
    CPM_DIR_SHIFT = 8,
};
typedef enum eCarPositionMarkerFlags eCarPositionMarkerFlags;
enum eConstraintType {
    eCONSTRAIN_BY_ROTATION = 0,
    eCONSTRAIN_BY_SCALE = 1,
};
typedef enum eConstraintType eConstraintType;
enum eContinueRaceType {
    CONTINUE_RACE_TYPE_NONE = 0,
    CONTINUE_RACE_TYPE_NEXT_RACE = 1,
    CONTINUE_RACE_TYPE_RESTART = 2,
};
typedef enum eContinueRaceType eContinueRaceType;
enum eFE_CUBIC_FLAGS {
    eFE_ONE_SPLINE = 0,
    eFE_RE_EVALUATE = 1,
};
typedef enum eFE_CUBIC_FLAGS eFE_CUBIC_FLAGS;
enum eFE_CUBIC_STATE {
    eFE_RESTING = 0,
    eFE_INPROGRESS = 1,
    eFE_NEWSPLINE = 2,
};
typedef enum eFE_CUBIC_STATE eFE_CUBIC_STATE;
enum eFILE_SYS_STATES {
    eFILE_SYS_IDLE = 0,
    eFILE_SYS_SAVE = 1,
    eFILE_SYS_LOAD = 2,
    eFILE_SYS_CHECK_EXISTENCE = 3,
    eFILE_SYS_GET_DIR = 4,
    eFILE_SYS_FORMAT = 5,
    eFILE_SYS_UNFORMAT = 6,
    eFILE_SYS_VERIFY = 7,
    eFILE_SYS_WAITING = 8,
    eFILE_SYS_TOTAL = 9,
};
typedef enum eFILE_SYS_STATES eFILE_SYS_STATES;
enum eFILE_SYS_SUBSTATES {
    eFILE_SYS_SUBSTATES_IDLE = 0,
    eFILE_SYS_CREATE_DIR = 1,
    eFILE_SYS_DELETE_ICONS = 2,
    eFILE_SYS_CREATE_ICON_SYS = 3,
    eFILE_SYS_CREATE_ICONS = 4,
    eFILE_SYS_CREATE_FILE = 5,
};
typedef enum eFILE_SYS_SUBSTATES eFILE_SYS_SUBSTATES;
enum eFILE_TYPE {
    eINITIAL_FILES = 0,
    eSETTINGS_FILE = 1,
};
typedef enum eFILE_TYPE eFILE_TYPE;
enum eLanguages {
    eLANGUAGE_NONE = -1,
    eLANGUAGE_FIRST = 0,
    eLANGUAGE_ENGLISH = 0,
    eLANGUAGE_FRENCH = 1,
    eLANGUAGE_GERMAN = 2,
    eLANGUAGE_ITALIAN = 3,
    eLANGUAGE_SPANISH = 4,
    eLANGUAGE_DUTCH = 5,
    eLANGUAGE_SWEDISH = 6,
    eLANGUAGE_LARGEST = 7,
    eLANGUAGE_LABELS = 8,
    eLANGUAGE_MAX = 9,
};
typedef enum eLanguages eLanguages;
enum eLightAttenuation {
    ELIGHT_ATTENUATION_NONE = 0,
    ELIGHT_ATTENUATION_NEAR = 1,
    ELIGHT_ATTENUATION_FAR = 2,
    ELIGHT_ATTENUATION_INVERSE = 4,
    ELIGHT_ATTENUATION_INVERSE_SQUARE = 8,
};
typedef enum eLightAttenuation eLightAttenuation;
enum eLightFlareFlags {
    ELF_BIDIRECTIONAL = 1,
};
typedef enum eLightFlareFlags eLightFlareFlags;
enum eLightFlareType {
    ELF_CAR_HEADLIGHT = 0,
    ELF_CAR_BRAKELIGHT = 1,
    ELF_CAR_REVERSELIGHT = 2,
    ELF_CAR_FOGLIGHT = 3,
    ELF_CAR_COPLIGHTRED = 4,
    ELF_CAR_COPLIGHTBLUE = 5,
    ELF_CAR_COPLIGHTWHITE = 6,
    ELF_CAR_COPHEADLIGHTRIGHT = 7,
    ELF_CAR_COPHEADLIGHTLEFT = 8,
    ELF_LAMPPOST = 9,
};
typedef enum eLightFlareType eLightFlareType;
enum eLightReflexionType {
    REF_NONE = 0,
    REF_TOPO = 1,
    REF_FAST = 2,
};
typedef enum eLightReflexionType eLightReflexionType;
enum eLightShape {
    ELIGHT_SHAPE_CIRCLE = 0,
    ELIGHT_SHAPE_RECTANGLE = 1,
    ELIGHT_SHAPE_SPHERE = 2,
    ELIGHT_SHAPE_AABB = 3,
};
typedef enum eLightShape eLightShape;
enum eLightState {
    ELIGHT_STATE_OFF = 0,
    ELIGHT_STATE_ON = 1,
};
typedef enum eLightState eLightState;
enum eLightType {
    ELIGHT_AMBIENT = 0,
    ELIGHT_FREE_DIRECTIONAL = 1,
    ELIGHT_OMNI = 2,
    ELIGHT_FREE_SPOT = 3,
    ELIGHT_EXCLUDE = 4,
    ELIGHT_NUM_LIGHTTYPES = 5,
};
typedef enum eLightType eLightType;
enum eMovementPhase {
    MP_GLOBAL_PHASE = 0,
    MP_COLLISION_DETECTION_PHASE = 1,
};
typedef enum eMovementPhase eMovementPhase;
enum eMoverType {
    MT_NONE = 0,
    MT_TEST = 1,
    MT_RIGIDBODY = 2,
    MT_ROCKET = 3,
    MT_ANIMATION = 4,
    MT_SPLINE = 5,
    MT_CAR = 6,
    MT_SEMIWHEEL = 7,
    MT_NUM_MOVER_TYPES = 8,
};
typedef enum eMoverType eMoverType;
enum ePhysicsUpgradeLevel {
    ePHYSICS_UPGRADE_LEVEL_0 = 0,
    ePHYSICS_UPGRADE_LEVEL_1 = 1,
    ePHYSICS_UPGRADE_LEVEL_2 = 2,
    ePHYSICS_UPGRADE_LEVEL_3 = 3,
    NUM_PHYSICS_UPGRADE_LEVELS = 4,
};
typedef enum ePhysicsUpgradeLevel ePhysicsUpgradeLevel;
enum ePhysicsUpgradePath {
    PUP_WEIGHT_REDUCTION = 0,
    PUP_SUSPENSION = 1,
    PUP_ENGINE = 2,
    PUP_TURBO = 3,
    PUP_NOS = 4,
    PUP_ECU = 5,
    PUP_TRANSMISSION = 6,
    PUP_TIRES = 7,
    PUP_BRAKE = 8,
    PUP_SPOILER = 9,
    PUP_GROUND_EFFECTS = 10,
    NUM_UPGRADE_PATHS = 11,
};
typedef enum ePhysicsUpgradePath ePhysicsUpgradePath;
enum eRenderFlags {
    ERF_NONE = 0,
    ERF_DISABLE_ENVMAP = 1,
    ERF_DISABLE_LIGHT = 2,
    ERF_FULL_VISIBLE = 4,
    ERF_NOT_VISIBLE = 8,
    ERF_FORCE_CLIP = 16,
    ERF_IGNORE_SOLID = 32,
    ERF_IGNORE_ALPHA = 64,
    ERF_FOG_VOLUME = 128,
    ERF_NO_PROJ_TEX = 256,
    ERF_PROJ_TEX = 512,
    ERF_ONLY_AMBIENT = 1024,
    ERF_ONLY_DIFFUSE = 2048,
    ERF_NO_REFLECTION = 4096,
};
typedef enum eRenderFlags eRenderFlags;
enum eReplaySpecialEvent {
    REPLAY_SPECIAL_EVENT_FORCE_RACE_FINISH = 0,
    REPLAY_SPECIAL_EVENT_DRIVER_MODE = 1,
    REPLAY_SPECIAL_EVENT_DAMAGE_MODE = 2,
    REPLAY_SPECIAL_EVENT_TRANSMISSION_TYPE = 3,
    REPLAY_SPECIAL_EVENT_JUMP_CAM = 4,
    REPLAY_SPECIAL_EVENT_ZONE_CAM = 5,
    REPLAY_SPECIAL_EVENT_SUPER_EASY_AI = 6,
    NUM_REPLAY_SPECIAL_EVENTS = 7,
};
typedef enum eReplaySpecialEvent eReplaySpecialEvent;
enum eSolidFlags {
    ESOLID_COMPRESSED_VERTS = 1,
    ESOLID_SHADOWMAP = 8,
    ESOLID_VERTEX_ANIMATION = 16,
    ESOLID_RANDOMIZE_START_FRAME = 32,
    ESOLID_ISLIT = 64,
    ESOLID_DUPLICATE_NAME = 128,
    ESOLID_DUPLICATE_NAME_ERROR = 256,
    ESOLID_DUPLICATED = 512,
};
typedef enum eSolidFlags eSolidFlags;
enum eSpecialSounds {
    SPECIAL_SOUND_HISTORY = 0,
    SPECIAL_SOUND_DIALOG_OPEN = 1,
    SPECIAL_SOUND_DIALOG_CLOSE = 2,
};
typedef enum eSpecialSounds eSpecialSounds;
enum eStomper {
    eSTOMPER_NONE = 0,
    eSTOMPER_CARTOON = 1,
    eSTOMPER_WIRE_ORANGE = 2,
    eSTOMPER_NEGATIVE_BLUE_FOG = 3,
    eSTOMPER_NEGATIVE_CREAM_FOG = 4,
    eSTOMPER_FLAT_ORANGE = 5,
    eSTOMPER_BLUE_NIGHT = 6,
    eSTOMPER_HOT_MAGENTA = 7,
    eSTOMPER_MOTION_BLUR = 8,
    eSTOMPER_BLACK_AND_WHITE = 9,
    eSTOMPER_COOL_FOG = 10,
    eSTOMPER_CUTIE_PETOOTIE = 11,
    eSTOMPER_CHROME_CARS = 12,
};
typedef enum eStomper eStomper;
enum eStomperTypes {
    eSTOMPER_DATA_INT8 = 0,
    eSTOMPER_DATA_INT16 = 1,
    eSTOMPER_DATA_INT32 = 2,
    eSTOMPER_DATA_FLOAT = 3,
    eSTOMPER_FUNCTION_INT = 4,
    eSTOMPER_FUNCTION_FLOAT = 5,
    eSTOMPER_NESTED = 6,
    eSTOMPER_END = 7,
};
typedef enum eStomperTypes eStomperTypes;
enum eStripDataFlags {
    ESDF_HAS_XYZ = 1,
    ESDF_HAS_RGBA = 2,
    ESDF_HAS_NORM = 4,
    ESDF_HAS_ST0 = 16,
    ESDF_HAS_ST1 = 32,
    ESDF_HAS_ST2 = 64,
    ESDF_HAS_ST3 = 128,
};
typedef enum eStripDataFlags eStripDataFlags;
enum eSuperBenderDebuggerLevel {
    SB_DEBUGGER_LEVEL_NONE = 0,
    SB_DEBUGGER_LEVEL_MID = 1,
    SB_DEBUGGER_LEVEL_FULL = 2,
};
typedef enum eSuperBenderDebuggerLevel eSuperBenderDebuggerLevel;
enum eVisibleState {
    EVISIBLESTATE_NOT = 0,
    EVISIBLESTATE_PARTIAL = 1,
    EVISIBLESTATE_FULL = 2,
};
typedef enum eVisibleState eVisibleState;
enum eVtxDesc {
    VTXDESC_DIRECT = 0,
    VTXDESC_I1 = 1,
    VTXDESC_I2 = 2,
    VTXDESC_NUM = 3,
};
typedef enum eVtxDesc eVtxDesc;
enum flareType {
    FLARE_NORM = 0,
    FLARE_ENV = 1,
    FLARE_REFLECT = 2,
};
typedef enum flareType flareType;
enum my_platform_types {
    MYPLAT_PS2 = 0,
    MYPLAT_XBOX = 1,
    MYPLAT_GAMECUBE = 2,
    MYPLAT_PC = 3,
};
typedef enum my_platform_types my_platform_types;

typedef struct ARAM_DLList ARAM_DLList;
typedef struct ARAM_DLNode ARAM_DLNode;
typedef struct ARAM_POOL ARAM_POOL;
typedef struct ARAM_POOL_MANAGER ARAM_POOL_MANAGER;
typedef struct ARGB ARGB;
typedef struct ARQRequest ARQRequest;
typedef struct ASinTableEntry ASinTableEntry;
typedef struct AcidActiveGroup AcidActiveGroup;
typedef struct AcidEffect AcidEffect;
typedef struct AcidEffectSystem AcidEffectSystem;
typedef struct AcidEmitter AcidEmitter;
typedef struct AcidEmitterData AcidEmitterData;
typedef struct AcidEmitterDataPackHeader AcidEmitterDataPackHeader;
typedef struct AcidEmitterPackHeader AcidEmitterPackHeader;
typedef struct AeroInfo AeroInfo;
typedef struct AllocationHeader AllocationHeader;
typedef struct AnimBank AnimBank;
typedef struct AnimMemoryMap AnimMemoryMap;
typedef struct AnimStat AnimStat;
typedef struct AnimTypeId AnimTypeId;
typedef struct AnimationMover AnimationMover;
typedef struct AnimationTarget AnimationTarget;
typedef struct Attribute Attribute;
typedef struct AttributeBlock AttributeBlock;
typedef struct AttributeDictionary AttributeDictionary;
typedef struct AttributeId AttributeId;
typedef struct Average Average;
typedef struct AverageBase AverageBase;
typedef struct AverageWindow AverageWindow;
typedef struct Avoidable Avoidable;
typedef struct BIGENTRY16 BIGENTRY16;
typedef struct BIGENTRY24 BIGENTRY24;
typedef struct BIGENTRY32 BIGENTRY32;
typedef struct BIGENTRY40 BIGENTRY40;
typedef struct BIGFileInfo BIGFileInfo;
typedef struct BIGHEADER BIGHEADER;
typedef struct BIGHEADER24 BIGHEADER24;
typedef struct BankStat BankStat;
typedef struct Base64 Base64;
typedef struct BodyTypeInfo BodyTypeInfo;
typedef struct BoneMask BoneMask;
typedef struct BrakeUpgradeInfo BrakeUpgradeInfo;
typedef struct CARDFileInfo CARDFileInfo;
typedef struct CARDStat CARDStat;
typedef struct CAnimCtrl CAnimCtrl;
typedef struct CAnimSkeleton CAnimSkeleton;
typedef struct CLUTHeader CLUTHeader;
typedef struct CONVERTER CONVERTER;
typedef struct COORD2 COORD2;
typedef struct COORD2_s16 COORD2_s16;
typedef struct COORD2_s8 COORD2_s8;
typedef struct COORD2_u16 COORD2_u16;
typedef struct COORD2_u8 COORD2_u8;
typedef struct COORD3 COORD3;
typedef struct COORD32 COORD32;
typedef struct COORD33 COORD33;
typedef struct COORD3_s16 COORD3_s16;
typedef struct COORD3_s8 COORD3_s8;
typedef struct COORD3_u16 COORD3_u16;
typedef struct COORD3_u8 COORD3_u8;
typedef struct COORD4 COORD4;
typedef struct CPUINFO CPUINFO;
typedef struct CachedRealFileHandle CachedRealFileHandle;
typedef struct Camera Camera;
typedef struct CameraDataItems CameraDataItems;
typedef struct CameraMover CameraMover;
typedef struct CameraNoiseParams CameraNoiseParams;
typedef struct CameraParams CameraParams;
typedef struct Car Car;
typedef struct CarPhysicsInfo CarPhysicsInfo;
typedef struct CarWorldObject CarWorldObject;
typedef struct CarWorldObjectMover CarWorldObjectMover;
typedef struct CharacterInfo CharacterInfo;
typedef struct Clan Clan;
typedef struct Class Class;
typedef struct ClassConstructorClient ClassConstructorClient;
typedef struct ClassDestructorClient ClassDestructorClient;
typedef struct ClassHandle ClassHandle;
typedef struct Collision Collision;
typedef struct CollisionSample CollisionSample;
typedef struct Colour Colour;
typedef struct CompleatCarPhysicsInfo CompleatCarPhysicsInfo;
typedef struct ComputationIndex ComputationIndex;
typedef struct ComputationIndexPrivateData ComputationIndexPrivateData;
typedef struct Condition Condition;
typedef struct Constant Constant;
typedef struct ConstructorPool ConstructorPool;
typedef struct Constructors Constructors;
typedef struct ControlInfo ControlInfo;
typedef struct DBInterface DBInterface;
typedef struct DEMOPadStatus DEMOPadStatus;
typedef struct DEMOStatObj DEMOStatObj;
typedef struct DSBranch DSBranch;
typedef struct DSCache DSCache;
typedef struct DSCacheNode DSCacheNode;
typedef struct DSHashTable DSHashTable;
typedef struct DSLink DSLink;
typedef struct DSList DSList;
typedef struct DSPADPCM DSPADPCM;
typedef struct DSTree DSTree;
typedef struct DVDCommandBlock DVDCommandBlock;
typedef struct DVDDir DVDDir;
typedef struct DVDDirEntry DVDDirEntry;
typedef struct DVDDiskID DVDDiskID;
typedef struct DVDFileInfo DVDFileInfo;
typedef struct DebugTopology DebugTopology;
typedef struct DefaultAnimBank DefaultAnimBank;
typedef struct DemoCameraCoordinator DemoCameraCoordinator;
typedef struct DemoDiscManager DemoDiscManager;
typedef struct DestructorEntry DestructorEntry;
typedef struct Device Device;
typedef struct DeviceExtension DeviceExtension;
typedef struct DevicePrivate DevicePrivate;
typedef struct DotWall DotWall;
typedef struct DriveTrainInfo DriveTrainInfo;
typedef struct DriverInfo DriverInfo;
typedef struct DummyFEngNewType DummyFEngNewType;
typedef struct DynamicCollisionBody DynamicCollisionBody;
typedef struct DynamicLoader DynamicLoader;
typedef struct EasterEggs EasterEggs;
typedef struct EasterEggsData EasterEggsData;
typedef struct ElipseTable ElipseTable;
typedef struct EndingNisCameraCoordinator EndingNisCameraCoordinator;
typedef struct EngineInfo EngineInfo;
typedef struct EngineUpgradeInfo EngineUpgradeInfo;
typedef struct EventHandler EventHandler;
typedef struct EventNode EventNode;
typedef struct EventSetup EventSetup;
typedef struct FEButtonMap FEButtonMap;
typedef struct FECodeListBox FECodeListBox;
typedef struct FEColor FEColor;
typedef struct FEEffect FEEffect;
typedef struct FEEvent FEEvent;
typedef struct FEEventList FEEventList;
typedef struct FEGameInterface FEGameInterface;
typedef struct FEGenericVal FEGenericVal;
typedef struct FEGroup FEGroup;
typedef struct FEHashList FEHashList;
typedef struct FEHashNode FEHashNode;
typedef struct FEHashNodePtr FEHashNodePtr;
typedef struct FEImage FEImage;
typedef struct FEKeyData FEKeyData;
typedef struct FEKeyNode FEKeyNode;
typedef struct FEKeyTrack FEKeyTrack;
typedef struct FELabelCallback FELabelCallback;
typedef struct FEList FEList;
typedef struct FEMatrix4 FEMatrix4;
typedef struct FEMessageResponse FEMessageResponse;
typedef struct FEMinList FEMinList;
typedef struct FEMinNode FEMinNode;
typedef struct FEModel FEModel;
typedef struct FEMovie FEMovie;
typedef struct FENode FENode;
typedef struct FEObjData FEObjData;
typedef struct FEObject FEObject;
typedef struct FEObjectDestructorCallback FEObjectDestructorCallback;
typedef struct FEPackage FEPackage;
typedef struct FEPoint FEPoint;
typedef struct FEQuaternion FEQuaternion;
typedef struct FERect FERect;
typedef struct FERefList FERefList;
typedef struct FEResponse FEResponse;
typedef struct FEScript FEScript;
typedef struct FEString FEString;
typedef struct FEVector2 FEVector2;
typedef struct FEVector3 FEVector3;
typedef struct FEVector4 FEVector4;
typedef struct FEWideString FEWideString;
typedef struct FILEBIG_def FILEBIG_def;
typedef struct FILEOPERATION_def FILEOPERATION_def;
typedef struct FILEREQUEST FILEREQUEST;
typedef struct FILESYSOPTS FILESYSOPTS;
typedef struct FONT FONT;
typedef struct FONTCHAR FONTCHAR;
typedef struct FONTCHARBUF FONTCHARBUF;
typedef struct FONTDRIVER FONTDRIVER;
typedef struct FONTDRIVEREXT FONTDRIVEREXT;
typedef struct FONTKERN FONTKERN;
typedef struct FS_EVNT_AS_READ_INFO FS_EVNT_AS_READ_INFO;
typedef struct FS_EVNT_CB_INFO FS_EVNT_CB_INFO;
typedef struct FS_EVNT_CD_READ_INFO FS_EVNT_CD_READ_INFO;
typedef struct FernFlattener FernFlattener;
typedef struct FernShaker FernShaker;
typedef struct FernShakerInfo FernShakerInfo;
typedef struct FloatColour FloatColour;
typedef struct FloatVector FloatVector;
typedef struct FlyByCameraCoordinator FlyByCameraCoordinator;
typedef struct FnAnimBank FnAnimBank;
typedef struct FnAnimFactory FnAnimFactory;
typedef struct FnAnimMemoryMap FnAnimMemoryMap;
typedef struct FnAnimSuper FnAnimSuper;
typedef struct FnDefaultAnimBank FnDefaultAnimBank;
typedef struct FogDistDef FogDistDef;
typedef struct FogVolume FogVolume;
typedef struct FogVolumeDef FogVolumeDef;
typedef struct Font Font;
typedef struct Force Force;
typedef struct FreeBlock FreeBlock;
typedef struct Function Function;
typedef struct FunctionClient FunctionClient;
typedef struct FunctionEntry FunctionEntry;
typedef struct FunctionHandle FunctionHandle;
typedef struct GCColourRGB565 GCColourRGB565;
typedef struct GCColourRGB8 GCColourRGB8;
typedef struct GCColourRGBA4 GCColourRGBA4;
typedef struct GCColourRGBA6 GCColourRGBA6;
typedef struct GFileDevice_def GFileDevice_def;
typedef struct GXFifoObj GXFifoObj;
typedef struct GameFlowManager GameFlowManager;
typedef struct GlobalVariable GlobalVariable;
typedef struct GlobalVariableHandle GlobalVariableHandle;
typedef struct GrandSceneryCullInfo GrandSceneryCullInfo;
typedef struct Graph Graph;
typedef struct GraphEntry_bVector2_ GraphEntry_bVector2_;
typedef struct GraphEntry_bVector3_ GraphEntry_bVector3_;
typedef struct GraphEntry_bVector4_ GraphEntry_bVector4_;
typedef struct GraphEntry_float_ GraphEntry_float_;
typedef struct GroundEffectsUpgradeInfo GroundEffectsUpgradeInfo;
typedef struct HANDLE_def HANDLE_def;
typedef struct IAllocator IAllocator;
typedef struct InitTerrainType InitTerrainType;
typedef struct Initializer Initializer;
typedef struct InterfaceId InterfaceId;
typedef struct IntroNisCameraCoordinator IntroNisCameraCoordinator;
typedef struct Irradiance Irradiance;
typedef struct JLZHash JLZHash;
typedef struct JLZHashPool JLZHashPool;
typedef struct JollyRancherResponsePacket JollyRancherResponsePacket;
typedef struct JoyData JoyData;
typedef struct Joylog Joylog;
typedef struct JoylogBuffer JoylogBuffer;
typedef struct Joystick Joystick;
typedef struct JunkmanUpgradeInfo JunkmanUpgradeInfo;
typedef struct LGConditionForceParams LGConditionForceParams;
typedef struct LGConstantForceParams LGConstantForceParams;
typedef struct LGEnvelopeParams LGEnvelopeParams;
typedef struct LGForceEffect LGForceEffect;
typedef struct LGPeriodicForceParams LGPeriodicForceParams;
typedef struct LGPosition LGPosition;
typedef struct LGRampForceParams LGRampForceParams;
typedef struct LGWheels LGWheels;
typedef struct LString LString;
typedef struct LZHeader LZHeader;
typedef struct LapSnapshotInfo LapSnapshotInfo;
typedef struct Linear Linear;
typedef struct ListPicker ListPicker;
typedef struct ListPickerItem ListPickerItem;
typedef struct LongVector LongVector;
typedef union MATRIX3 MATRIX3;
typedef union MATRIX4 MATRIX4;
typedef struct MEMCLASS MEMCLASS;
typedef struct MUTEX MUTEX;
typedef struct MagazineBackground MagazineBackground;
typedef struct MagazineCamera MagazineCamera;
typedef struct MagazineEvent MagazineEvent;
typedef struct MagazineFemale MagazineFemale;
typedef struct MagazineHeadline MagazineHeadline;
typedef struct MagazineName MagazineName;
typedef struct MagazineName2ThumbMapping MagazineName2ThumbMapping;
typedef struct MagazineScreenDebugData MagazineScreenDebugData;
typedef struct MagazineSetup MagazineSetup;
typedef struct MagazineTemplateDatabase MagazineTemplateDatabase;
typedef struct MatchPhaseInput MatchPhaseInput;
typedef struct MemBlockInfo_t MemBlockInfo_t;
typedef struct MemoryPool MemoryPool;
typedef struct MemoryPoolManager MemoryPoolManager;
typedef struct MenuScreen MenuScreen;
typedef struct MiscCarTunings MiscCarTunings;
typedef struct MobileTopology MobileTopology;
typedef struct Mover Mover;
typedef struct MtxStack MtxStack;
typedef struct NitrousInfo NitrousInfo;
typedef struct NitrousUpgradeInfo NitrousUpgradeInfo;
typedef union OP_def OP_def;
typedef struct OSAlarm OSAlarm;
typedef struct OSCalendarTime OSCalendarTime;
typedef struct OSCond OSCond;
typedef struct OSContext OSContext;
typedef struct OSFontHeader OSFontHeader;
typedef struct OSImportInfo OSImportInfo;
typedef struct OSMessageQueue OSMessageQueue;
typedef struct OSModuleHeader OSModuleHeader;
typedef struct OSModuleInfo OSModuleInfo;
typedef struct OSModuleLink OSModuleLink;
typedef struct OSModuleQueue OSModuleQueue;
typedef struct OSMutex OSMutex;
typedef struct OSMutexLink OSMutexLink;
typedef struct OSMutexQueue OSMutexQueue;
typedef struct OSRel OSRel;
typedef struct OSResetFunctionInfo OSResetFunctionInfo;
typedef struct OSSectionInfo OSSectionInfo;
typedef struct OSSemaphore OSSemaphore;
typedef struct OSStopwatch OSStopwatch;
typedef struct OSThread OSThread;
typedef struct OSThreadLink OSThreadLink;
typedef struct OSThreadQueue OSThreadQueue;
typedef struct ObjectPool_FEKeyNode_256_ ObjectPool_FEKeyNode_256_;
typedef struct ObjectPool_FEMessageResponse_64_ ObjectPool_FEMessageResponse_64_;
typedef struct ObjectPool_FEScript_32_ ObjectPool_FEScript_32_;
typedef struct PADStatus PADStatus;
typedef struct PPC_DMA_L_t PPC_DMA_L_t;
typedef union PPC_DMA_L_u PPC_DMA_L_u;
typedef struct PPC_DMA_U_t PPC_DMA_U_t;
typedef union PPC_DMA_U_u PPC_DMA_U_u;
typedef struct PPC_GQR_t PPC_GQR_t;
typedef union PPC_GQR_u PPC_GQR_u;
typedef struct PadData PadData;
typedef union Parameter Parameter;
typedef struct PerfMeter PerfMeter;
typedef struct Periodic Periodic;
typedef struct PersistentMenuScreenData PersistentMenuScreenData;
typedef struct PhaseChan PhaseChan;
typedef struct PhaseValue PhaseValue;
typedef struct PhysicsSystem PhysicsSystem;
typedef struct PhysicsUpgradeSpecification PhysicsUpgradeSpecification;
typedef struct PidError PidError;
typedef struct PostRaceCameraCoordinator PostRaceCameraCoordinator;
typedef struct PreRaceCameraCoordinator PreRaceCameraCoordinator;
typedef struct PreviewRoute PreviewRoute;
typedef struct ProfileDataEntry ProfileDataEntry;
typedef struct ProfileManager ProfileManager;
typedef struct ProfileNode ProfileNode;
typedef struct ProfileSectionEntry ProfileSectionEntry;
typedef struct Quaternion Quaternion;
typedef struct QueuedFile QueuedFile;
typedef struct QueuedFileBundle QueuedFileBundle;
typedef struct QuickSpline QuickSpline;
typedef struct RCQueuedMessage RCQueuedMessage;
typedef struct REALELEMENT REALELEMENT;
typedef struct REALQUEUE REALQUEUE;
typedef struct REQUESTQUEUE REQUESTQUEUE;
typedef struct REQUESTSTRUCTtag REQUESTSTRUCTtag;
typedef struct RaceCoordinator RaceCoordinator;
typedef struct Ramp Ramp;
typedef struct ReadStatus ReadStatus;
typedef struct RecyclingThumbnailNode RecyclingThumbnailNode;
typedef struct RecyclingThumbnailScroller RecyclingThumbnailScroller;
typedef struct RenderContext RenderContext;
typedef struct RenderContextBase RenderContextBase;
typedef struct RenderContextExtension RenderContextExtension;
typedef struct RenderContextPrivate RenderContextPrivate;
typedef struct RenderMethod RenderMethod;
typedef struct ReplayManager ReplayManager;
typedef struct ReplayScreen ReplayScreen;
typedef struct ReplaySnapshot ReplaySnapshot;
typedef struct ReplaySnapshotSectionInfo ReplaySnapshotSectionInfo;
typedef struct ReplaySpecialEventInfo ReplaySpecialEventInfo;
typedef struct RigidBody RigidBody;
typedef struct RigidBodyInfo RigidBodyInfo;
typedef struct RigidBodyMover RigidBodyMover;
typedef struct RuntimeAllocConstructorPool RuntimeAllocConstructorPool;
typedef struct RuntimeAllocDestructorEntry RuntimeAllocDestructorEntry;
typedef struct S16Vec S16Vec;
typedef struct SCATTERREQUEST SCATTERREQUEST;
typedef struct SCOORD SCOORD;
typedef struct SHAPE SHAPE;
typedef struct SHAPEFILE SHAPEFILE;
typedef struct SHAPETABLEENTRY SHAPETABLEENTRY;
typedef struct SIGNAL SIGNAL;
typedef union SN_PS_type SN_PS_type;
typedef struct STREAMCHUNKHDR STREAMCHUNKHDR;
typedef struct STRUCT_DSP_TASK STRUCT_DSP_TASK;
typedef struct SYNC_TRANSFER_OP SYNC_TRANSFER_OP;
typedef struct SampleMover SampleMover;
typedef struct ScannerConfig ScannerConfig;
typedef struct SceneryBoundingBox SceneryBoundingBox;
typedef struct SceneryCullInfo SceneryCullInfo;
typedef struct SceneryDrawInfo SceneryDrawInfo;
typedef struct SceneryInfo SceneryInfo;
typedef struct SceneryInstance SceneryInstance;
typedef struct SceneryLogInfo SceneryLogInfo;
typedef struct ScenerySectionHeader ScenerySectionHeader;
typedef struct SceneryTreeGroupNode SceneryTreeGroupNode;
typedef struct SceneryTreeNode SceneryTreeNode;
typedef struct ScreenConstructorData ScreenConstructorData;
typedef struct ScrollerDatum ScrollerDatum;
typedef struct ScrollerDatumNode ScrollerDatumNode;
typedef struct ScrollerSlot ScrollerSlot;
typedef struct ScrollerSlotNode ScrollerSlotNode;
typedef struct Scrollerina Scrollerina;
typedef struct ScrollyLogic ScrollyLogic;
typedef struct SemiWheelMover SemiWheelMover;
typedef struct SetupStats SetupStats;
typedef struct Skeleton Skeleton;
typedef struct SkeletonData SkeletonData;
typedef struct SkidMaker SkidMaker;
typedef struct SkidSegment SkidSegment;
typedef struct SkidSet SkidSet;
typedef struct SlotPool SlotPool;
typedef struct SlotPoolEntry SlotPoolEntry;
typedef struct SlotPoolManager SlotPoolManager;
typedef struct SmokeableInfo SmokeableInfo;
typedef struct SoundImplementer SoundImplementer;
typedef struct SoundObject SoundObject;
typedef struct SpaceCadet SpaceCadet;
typedef struct SpaceCase SpaceCase;
typedef struct SpaceNode SpaceNode;
typedef struct Spline Spline;
typedef struct SpoilerUpgradeInfo SpoilerUpgradeInfo;
typedef struct State State;
typedef struct StateTest StateTest;
typedef struct StompEntry StompEntry;
typedef struct Stomper Stomper;
typedef struct SubTitleDesc SubTitleDesc;
typedef struct SuspensionInfo SuspensionInfo;
typedef struct SuspensionUpgradeInfo SuspensionUpgradeInfo;
typedef struct Symbol Symbol;
typedef struct SymbolEntry SymbolEntry;
typedef struct SymbolPool SymbolPool;
typedef struct System System;
typedef struct TAR TAR;
typedef struct TARExtension TARExtension;
typedef struct TARPrivate TARPrivate;
typedef struct TEXDescriptor TEXDescriptor;
typedef struct TEXHeader TEXHeader;
typedef struct TEXPalette TEXPalette;
typedef struct THREAD THREAD;
typedef struct Table Table;
typedef struct TableBase TableBase;
typedef struct TerrainEffectEmitters TerrainEffectEmitters;
typedef struct TerrainTypeInfo TerrainTypeInfo;
typedef struct TestMover TestMover;
typedef struct TextureAnim TextureAnim;
typedef struct TextureAnimEntry TextureAnimEntry;
typedef struct TextureAnimPack TextureAnimPack;
typedef struct TextureAnimPackHeader TextureAnimPackHeader;
typedef struct TextureIndexEntry TextureIndexEntry;
typedef struct TextureInfoPlat TextureInfoPlat;
typedef struct TextureInfoPlatInfo TextureInfoPlatInfo;
typedef struct TextureInfoPlatInfoOBJ TextureInfoPlatInfoOBJ;
typedef struct TextureInfoPlatInterface TextureInfoPlatInterface;
typedef struct TexturePack TexturePack;
typedef struct TexturePackHeader TexturePackHeader;
typedef struct TextureRenderContext TextureRenderContext;
typedef struct ThumbObject ThumbObject;
typedef struct ThumbnailNode ThumbnailNode;
typedef struct ThumbnailScroller ThumbnailScroller;
typedef struct Timer Timer;
typedef struct TireInfo TireInfo;
typedef struct TireUpgradeInfo TireUpgradeInfo;
typedef struct TopologyCoordinate TopologyCoordinate;
typedef struct TopologyEntry TopologyEntry;
typedef struct TopologyTree TopologyTree;
typedef struct TopologyTreeFilePolygon TopologyTreeFilePolygon;
typedef struct TopologyTreeNode TopologyTreeNode;
typedef struct TopologyTreePolygon TopologyTreePolygon;
typedef struct TopologyTreeSquare TopologyTreeSquare;
typedef struct TrackHeader TrackHeader;
typedef struct TrackPolygon TrackPolygon;
typedef struct TrackPolygonCollisionBody TrackPolygonCollisionBody;
typedef struct TrackPositionMarker TrackPositionMarker;
typedef struct TrackRoutePoint TrackRoutePoint;
typedef struct TrainInfo TrainInfo;
typedef struct Transform Transform;
typedef struct TransmissionUpgradeInfo TransmissionUpgradeInfo;
typedef struct TurboUpgradeInfo TurboUpgradeInfo;
typedef struct ValidJoystickConfiguration ValidJoystickConfiguration;
typedef struct Vec Vec;
typedef struct VerbosityControl VerbosityControl;
typedef struct ViewPort ViewPort;
typedef struct VisibleData VisibleData;
typedef struct WeatherQuery WeatherQuery;
typedef struct WeatherRegion WeatherRegion;
typedef struct WeatherRegionPack WeatherRegionPack;
typedef struct WeightReductionUpgradeInfo WeightReductionUpgradeInfo;
typedef struct WheelInfo WheelInfo;
typedef struct Wheels Wheels;
typedef struct WorldEntity WorldEntity;
typedef struct WorldEntityList WorldEntityList;
typedef struct WorldEntityRegistration WorldEntityRegistration;
typedef struct WorldEntityType WorldEntityType;
typedef struct WorldModel WorldModel;
typedef struct WorldObjectAnimationInfo WorldObjectAnimationInfo;
typedef struct WorldObjectCollisionBody WorldObjectCollisionBody;
typedef struct WorldObjectInfo WorldObjectInfo;
typedef struct _AXPB _AXPB;
typedef struct _AXPBADDR _AXPBADDR;
typedef struct _AXPBADPCM _AXPBADPCM;
typedef struct _AXPBADPCMLOOP _AXPBADPCMLOOP;
typedef struct _AXPBDPOP _AXPBDPOP;
typedef struct _AXPBFIR _AXPBFIR;
typedef struct _AXPBITD _AXPBITD;
typedef struct _AXPBITDBUFFER _AXPBITDBUFFER;
typedef struct _AXPBLPF _AXPBLPF;
typedef struct _AXPBMIX _AXPBMIX;
typedef struct _AXPBSRC _AXPBSRC;
typedef struct _AXPBU _AXPBU;
typedef struct _AXPBUPDATE _AXPBUPDATE;
typedef struct _AXPBVE _AXPBVE;
typedef struct _AXPROFILE _AXPROFILE;
typedef struct _AXSPB _AXSPB;
typedef struct _AXVPB _AXVPB;
typedef struct _Bigint _Bigint;
typedef struct _GXColor _GXColor;
typedef struct _GXColorS10 _GXColorS10;
typedef struct _GXFogAdjTable _GXFogAdjTable;
typedef struct _GXLightObj _GXLightObj;
typedef struct _GXRenderModeObj _GXRenderModeObj;
typedef struct _GXTexObj _GXTexObj;
typedef struct _GXTexRegion _GXTexRegion;
typedef struct _GXTlutObj _GXTlutObj;
typedef struct _GXTlutRegion _GXTlutRegion;
typedef struct _GXVtxAttrFmtList _GXVtxAttrFmtList;
typedef struct _GXVtxDescList _GXVtxDescList;
typedef union __dmath __dmath;
typedef struct __exception __exception;
typedef struct __fmath_type __fmath_type;
typedef struct __sFILE __sFILE;
typedef struct __sbuf __sbuf;
typedef struct __va_list_tag __va_list_tag;
typedef struct __va_regsave_t __va_regsave_t;
typedef struct __vtbl_ptr_type __vtbl_ptr_type;
typedef struct _atexit _atexit;
typedef struct _glue _glue;
typedef struct _reent _reent;
typedef struct _types_fd_set _types_fd_set;
typedef struct bChunk bChunk;
typedef struct bFile bFile;
typedef struct bFileCallbackEntry bFileCallbackEntry;
typedef struct bFileClosePacket bFileClosePacket;
typedef struct bFileDirectoryEntry bFileDirectoryEntry;
typedef struct bFileOpenPacket bFileOpenPacket;
typedef struct bFileReadPacket bFileReadPacket;
typedef struct bFileWritePacket bFileWritePacket;
typedef struct bFixVector2 bFixVector2;
typedef struct bFixVector3 bFixVector3;
typedef struct bFunkCodeineAddServerPacket bFunkCodeineAddServerPacket;
typedef struct bFunkJoylogCaptureInitPacket bFunkJoylogCaptureInitPacket;
typedef struct bFunkPacket bFunkPacket;
typedef struct bFunkPacketHeader bFunkPacketHeader;
typedef struct bFunkServer bFunkServer;
typedef struct bFunkServerPlatform bFunkServerPlatform;
typedef struct bList bList;
typedef struct bMatrix4 bMatrix4;
typedef struct bMemoryTraceAllocatePacket bMemoryTraceAllocatePacket;
typedef struct bMemoryTraceDeletePoolPacket bMemoryTraceDeletePoolPacket;
typedef struct bMemoryTraceFreePacket bMemoryTraceFreePacket;
typedef struct bMemoryTraceNewPoolPacket bMemoryTraceNewPoolPacket;
typedef struct bNode bNode;
typedef struct bPList_QuickSpline_ bPList_QuickSpline_;
typedef struct bPList_QuickSpline__bTList_bPNode_ bPList_QuickSpline__bTList_bPNode_;
typedef struct bPList_SkidSet_ bPList_SkidSet_;
typedef struct bPList_SkidSet__bTList_bPNode_ bPList_SkidSet__bTList_bPNode_;
typedef struct bPNode bPNode;
typedef struct bPolar bPolar;
typedef struct bQuaternion bQuaternion;
typedef struct bRepeat bRepeat;
typedef struct bRepeatChannel bRepeatChannel;
typedef struct bSpline bSpline;
typedef struct bTList_AcidActiveGroup_ bTList_AcidActiveGroup_;
typedef struct bTList_AcidEmitter_ bTList_AcidEmitter_;
typedef struct bTList_AllocationHeader_ bTList_AllocationHeader_;
typedef struct bTList_CAnimSkeleton_ bTList_CAnimSkeleton_;
typedef struct bTList_CachedRealFileHandle_ bTList_CachedRealFileHandle_;
typedef struct bTList_CameraMover_ bTList_CameraMover_;
typedef struct bTList_Clan_ bTList_Clan_;
typedef struct bTList_DebugTopology_ bTList_DebugTopology_;
typedef struct bTList_EventSetup_ bTList_EventSetup_;
typedef struct bTList_FernFlattener_ bTList_FernFlattener_;
typedef struct bTList_FernShaker_ bTList_FernShaker_;
typedef struct bTList_FreeBlock_ bTList_FreeBlock_;
typedef struct bTList_ListPicker__ListPickerItem_ bTList_ListPicker__ListPickerItem_;
typedef struct bTList_MagazineBackground_ bTList_MagazineBackground_;
typedef struct bTList_MagazineCamera_ bTList_MagazineCamera_;
typedef struct bTList_MagazineEvent_ bTList_MagazineEvent_;
typedef struct bTList_MagazineFemale_ bTList_MagazineFemale_;
typedef struct bTList_MagazineHeadline_ bTList_MagazineHeadline_;
typedef struct bTList_MagazineName_ bTList_MagazineName_;
typedef struct bTList_MagazineSetup_ bTList_MagazineSetup_;
typedef struct bTList_Mover_ bTList_Mover_;
typedef struct bTList_QueuedFile_ bTList_QueuedFile_;
typedef struct bTList_RCQueuedMessage_ bTList_RCQueuedMessage_;
typedef struct bTList_RecyclingThumbnailNode_ bTList_RecyclingThumbnailNode_;
typedef struct bTList_ReplaySnapshot_ bTList_ReplaySnapshot_;
typedef struct bTList_ScrollerDatumNode_ bTList_ScrollerDatumNode_;
typedef struct bTList_ScrollerDatum_ bTList_ScrollerDatum_;
typedef struct bTList_ScrollerSlotNode_ bTList_ScrollerSlotNode_;
typedef struct bTList_ScrollerSlot_ bTList_ScrollerSlot_;
typedef struct bTList_SetupStats_ bTList_SetupStats_;
typedef struct bTList_SlotPool_ bTList_SlotPool_;
typedef struct bTList_SpaceNode_ bTList_SpaceNode_;
typedef struct bTList_ThumbObject_ bTList_ThumbObject_;
typedef struct bTList_ThumbnailNode_ bTList_ThumbnailNode_;
typedef struct bTList_TrackPolygon_ bTList_TrackPolygon_;
typedef struct bTList_WeatherRegion_ bTList_WeatherRegion_;
typedef struct bTList_WorldEntityList_ bTList_WorldEntityList_;
typedef struct bTList_WorldEntityType_ bTList_WorldEntityType_;
typedef struct bTList_WorldEntity_ bTList_WorldEntity_;
typedef struct bTList_WorldModel_ bTList_WorldModel_;
typedef struct bTList_bFileCallbackEntry_ bTList_bFileCallbackEntry_;
typedef struct bTList_bPNode_ bTList_bPNode_;
typedef struct bTList_eDynamicLight_ bTList_eDynamicLight_;
typedef struct bTList_eLightFlare_ bTList_eLightFlare_;
typedef struct bTList_eModel_ bTList_eModel_;
typedef struct bTList_eSolid_ bTList_eSolid_;
typedef struct bTNode_AcidEmitterData_ bTNode_AcidEmitterData_;
typedef struct bTNode_AcidEmitter_ bTNode_AcidEmitter_;
typedef struct bTNode_AllocationHeader_ bTNode_AllocationHeader_;
typedef struct bTNode_CAnimSkeleton_ bTNode_CAnimSkeleton_;
typedef struct bTNode_CachedRealFileHandle_ bTNode_CachedRealFileHandle_;
typedef struct bTNode_Clan_ bTNode_Clan_;
typedef struct bTNode_DebugTopology_ bTNode_DebugTopology_;
typedef struct bTNode_EventSetup_ bTNode_EventSetup_;
typedef struct bTNode_FernFlattener_ bTNode_FernFlattener_;
typedef struct bTNode_FernShaker_ bTNode_FernShaker_;
typedef struct bTNode_Font_ bTNode_Font_;
typedef struct bTNode_FreeBlock_ bTNode_FreeBlock_;
typedef struct bTNode_ListPicker__ListPickerItem_ bTNode_ListPicker__ListPickerItem_;
typedef struct bTNode_MagazineBackground_ bTNode_MagazineBackground_;
typedef struct bTNode_MagazineCamera_ bTNode_MagazineCamera_;
typedef struct bTNode_MagazineEvent_ bTNode_MagazineEvent_;
typedef struct bTNode_MagazineFemale_ bTNode_MagazineFemale_;
typedef struct bTNode_MagazineHeadline_ bTNode_MagazineHeadline_;
typedef struct bTNode_MagazineName_ bTNode_MagazineName_;
typedef struct bTNode_MagazineSetup_ bTNode_MagazineSetup_;
typedef struct bTNode_Mover_ bTNode_Mover_;
typedef struct bTNode_QueuedFile_ bTNode_QueuedFile_;
typedef struct bTNode_RCQueuedMessage_ bTNode_RCQueuedMessage_;
typedef struct bTNode_RecyclingThumbnailNode_ bTNode_RecyclingThumbnailNode_;
typedef struct bTNode_ReplaySnapshot_ bTNode_ReplaySnapshot_;
typedef struct bTNode_ScenerySectionHeader_ bTNode_ScenerySectionHeader_;
typedef struct bTNode_ScrollerDatumNode_ bTNode_ScrollerDatumNode_;
typedef struct bTNode_ScrollerDatum_ bTNode_ScrollerDatum_;
typedef struct bTNode_ScrollerSlotNode_ bTNode_ScrollerSlotNode_;
typedef struct bTNode_ScrollerSlot_ bTNode_ScrollerSlot_;
typedef struct bTNode_SetupStats_ bTNode_SetupStats_;
typedef struct bTNode_SkidSet_ bTNode_SkidSet_;
typedef struct bTNode_SlotPool_ bTNode_SlotPool_;
typedef struct bTNode_SpaceNode_ bTNode_SpaceNode_;
typedef struct bTNode_TextureAnimPack_ bTNode_TextureAnimPack_;
typedef struct bTNode_TextureInfoPlatInfo_ bTNode_TextureInfoPlatInfo_;
typedef struct bTNode_TextureInfo_ bTNode_TextureInfo_;
typedef struct bTNode_TexturePack_ bTNode_TexturePack_;
typedef struct bTNode_ThumbObject_ bTNode_ThumbObject_;
typedef struct bTNode_ThumbnailNode_ bTNode_ThumbnailNode_;
typedef struct bTNode_TrackPolygon_ bTNode_TrackPolygon_;
typedef struct bTNode_TrackPositionMarker_ bTNode_TrackPositionMarker_;
typedef struct bTNode_WeatherRegionPack_ bTNode_WeatherRegionPack_;
typedef struct bTNode_WeatherRegion_ bTNode_WeatherRegion_;
typedef struct bTNode_WorldEntityType_ bTNode_WorldEntityType_;
typedef struct bTNode_WorldEntity_ bTNode_WorldEntity_;
typedef struct bTNode_WorldModel_ bTNode_WorldModel_;
typedef struct bTNode_bFileCallbackEntry_ bTNode_bFileCallbackEntry_;
typedef struct bTNode_bFunkServer_ bTNode_bFunkServer_;
typedef struct bTNode_bPNode_ bTNode_bPNode_;
typedef struct bTNode_eDynamicFrameMemory_ bTNode_eDynamicFrameMemory_;
typedef struct bTNode_eDynamicLightPack_ bTNode_eDynamicLightPack_;
typedef struct bTNode_eDynamicLight_ bTNode_eDynamicLight_;
typedef struct bTNode_eLightFlarePackHeader_ bTNode_eLightFlarePackHeader_;
typedef struct bTNode_eLightFlare_ bTNode_eLightFlare_;
typedef struct bTNode_eLightMaterial_ bTNode_eLightMaterial_;
typedef struct bTNode_eLightPack_ bTNode_eLightPack_;
typedef struct bTNode_eModel_ bTNode_eModel_;
typedef struct bTNode_ePoly_ bTNode_ePoly_;
typedef struct bTNode_eSolidListHeader_ bTNode_eSolidListHeader_;
typedef struct bTNode_eSolid_ bTNode_eSolid_;
typedef struct bVU0Float bVU0Float;
typedef struct bVector2 bVector2;
typedef struct bVector3 bVector3;
typedef struct bVector3Image bVector3Image;
typedef struct bVector3Reg bVector3Reg;
typedef struct bVector4 bVector4;
typedef struct bVirtualDestructNode bVirtualDestructNode;
typedef struct bad_alloc bad_alloc;
typedef struct cCaptureBuffer cCaptureBuffer;
typedef struct cDBFileSys cDBFileSys;
typedef struct cDBFilesBase cDBFilesBase;
typedef struct cFEngRender cFEngRender;
typedef struct cMC cMC;
typedef struct cPoint cPoint;
typedef struct cPostRaceScreenMgrInterface cPostRaceScreenMgrInterface;
typedef struct cQuarterSizeMap cQuarterSizeMap;
typedef struct cReflectMap cReflectMap;
typedef struct cSphereMap cSphereMap;
typedef struct div_t div_t;
typedef struct eAnimationSolid eAnimationSolid;
typedef struct eAnimationTree eAnimationTree;
typedef struct eAnimationTreeNode eAnimationTreeNode;
typedef struct eDamageInfo eDamageInfo;
typedef struct eDamageVertex eDamageVertex;
typedef struct eDataRenderDynamic eDataRenderDynamic;
typedef struct eDmaChain eDmaChain;
typedef struct eDmaData eDmaData;
typedef struct eDmaDataList eDmaDataList;
typedef struct eDynamicFrameMemory eDynamicFrameMemory;
typedef struct eDynamicLightPack eDynamicLightPack;
typedef struct eEnvMap eEnvMap;
typedef struct eFilter eFilter;
typedef struct eFogParams eFogParams;
typedef struct eLight eLight;
typedef struct eLightContext eLightContext;
typedef struct eLightFlare eLightFlare;
typedef struct eLightFlarePackHeader eLightFlarePackHeader;
typedef struct eLightMaterialData eLightMaterialData;
typedef struct eLightMaterialEntry eLightMaterialEntry;
typedef struct eLightMaterialPlatInfo eLightMaterialPlatInfo;
typedef struct eLightMaterialPlatInterface eLightMaterialPlatInterface;
typedef struct eLightPack eLightPack;
typedef struct eModel eModel;
typedef struct eNormalSmoother eNormalSmoother;
typedef struct ePointSprite3D ePointSprite3D;
typedef struct ePoly ePoly;
typedef struct ePositionMarker ePositionMarker;
typedef struct eRenderTarget eRenderTarget;
typedef struct eReplacementTextureTable eReplacementTextureTable;
typedef struct eRigidAnimation eRigidAnimation;
typedef struct eSmoothVertex eSmoothVertex;
typedef struct eSmoothVertexPlat eSmoothVertexPlat;
typedef struct eSolidIndexEntry eSolidIndexEntry;
typedef struct eSolidListHeader eSolidListHeader;
typedef struct eSolidPlatInfo eSolidPlatInfo;
typedef struct eSolidPlatInterface eSolidPlatInterface;
typedef struct eStreamingEntry eStreamingEntry;
typedef struct eStripData eStripData;
typedef struct eStripDataE eStripDataE;
typedef struct eStripEntry eStripEntry;
typedef struct eTextureBucket eTextureBucket;
typedef struct eTextureEntry eTextureEntry;
typedef struct eVertexAnimator eVertexAnimator;
typedef struct eView eView;
typedef struct eViewPlatInfo eViewPlatInfo;
typedef struct eViewPlatInterface eViewPlatInterface;
typedef struct eflock eflock;
typedef struct fMatrix4 fMatrix4;
typedef struct fVector2 fVector2;
typedef struct fVector3 fVector3;
typedef struct fVector4 fVector4;
typedef struct fVectorW fVectorW;
typedef struct fVectorX fVectorX;
typedef struct fVectorY fVectorY;
typedef struct fVectorZ fVectorZ;
typedef struct flock flock;
typedef struct ldiv_t ldiv_t;
typedef struct sepdef sepdef;
typedef struct stat stat;
typedef struct tCubic1D tCubic1D;
typedef struct tCubic2D tCubic2D;
typedef struct tCubic3D tCubic3D;
typedef struct tFESettingsFile tFESettingsFile;
typedef struct tGraph_bVector2_ tGraph_bVector2_;
typedef struct tGraph_bVector3_ tGraph_bVector3_;
typedef struct tGraph_bVector4_ tGraph_bVector4_;
typedef struct tGraph_float_ tGraph_float_;
typedef struct tTTFontHeader tTTFontHeader;
typedef struct tTTGlyph tTTGlyph;
typedef struct tTTKerningPair tTTKerningPair;
typedef struct tTable_bVector2_ tTable_bVector2_;
typedef struct tTable_bVector3_ tTable_bVector3_;
typedef struct tTable_bVector4_ tTable_bVector4_;
typedef struct tTable_float_ tTable_float_;
typedef struct tm tm;
typedef union uPPCWGPipe uPPCWGPipe;
typedef struct vAABB vAABB;
typedef struct vAABBTree vAABBTree;
typedef struct vSphere vSphere;

// ---- typedefs ----
typedef char int8;
typedef unsigned char uint8;
typedef short int16;
typedef unsigned short uint16;
typedef int int32;
typedef unsigned int uint32;
typedef long long int64;
typedef unsigned long long uint64;
typedef unsigned int type_operator_new;
typedef void * CodeineReloadExeFunction(int);
typedef int bFix;
typedef unsigned short bAngle;
typedef char s8;
typedef short s16;
typedef int s32;
typedef long long s64;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef volatile unsigned char vu8;
typedef volatile unsigned short vu16;
typedef volatile unsigned long vu32;
typedef volatile unsigned long long vu64;
typedef volatile signed char vs8;
typedef volatile short vs16;
typedef volatile long vs32;
typedef volatile long long vs64;
typedef float f32;
typedef double f64;
typedef volatile float vf32;
typedef volatile double vf64;
typedef char * Ptr;
typedef int BOOL;
typedef unsigned char GXBool;
typedef enum _GXAttr GXAttr;
typedef enum _GXVtxFmt GXVtxFmt;
typedef enum _GXAttrType GXAttrType;
typedef enum _GXTexCoordID GXTexCoordID;
typedef enum _GXTexGenType GXTexGenType;
typedef enum _GXTexGenSrc GXTexGenSrc;
typedef enum _GXCompCnt GXCompCnt;
typedef enum _GXCompType GXCompType;
typedef enum _GXChannelID GXChannelID;
typedef enum _GXColorSrc GXColorSrc;
typedef enum _GXLightID GXLightID;
typedef enum _GXDiffuseFn GXDiffuseFn;
typedef enum _GXAttnFn GXAttnFn;
typedef enum _GXSpotFn GXSpotFn;
typedef enum _GXDistAttnFn GXDistAttnFn;
typedef enum _GXPosNrmMtx GXPosNrmMtx;
typedef enum _GXTexMtx GXTexMtx;
typedef enum _GXPTTexMtx GXPTTexMtx;
typedef enum _GXTexMtxType GXTexMtxType;
typedef enum _GXPrimitive GXPrimitive;
typedef enum _GXTexOffset GXTexOffset;
typedef enum _GXCullMode GXCullMode;
typedef enum _GXClipMode GXClipMode;
typedef enum _GXTexWrapMode GXTexWrapMode;
typedef enum _GXTexFilter GXTexFilter;
typedef enum _GXCITexFmt GXCITexFmt;
typedef enum _GXTexFmt GXTexFmt;
typedef enum _GXTlutFmt GXTlutFmt;
typedef enum _GXTlutSize GXTlutSize;
typedef enum _GXTlut GXTlut;
typedef enum _GXTexMapID GXTexMapID;
typedef enum _GXTexCacheSize GXTexCacheSize;
typedef enum _GXIndTexFormat GXIndTexFormat;
typedef enum _GXIndTexBiasSel GXIndTexBiasSel;
typedef enum _GXIndTexAlphaSel GXIndTexAlphaSel;
typedef enum _GXIndTexMtxID GXIndTexMtxID;
typedef enum _GXIndTexWrap GXIndTexWrap;
typedef enum _GXIndTexScale GXIndTexScale;
typedef enum _GXIndTexStageID GXIndTexStageID;
typedef enum _GXTevStageID GXTevStageID;
typedef enum _GXTevRegID GXTevRegID;
typedef enum _GXTevOp GXTevOp;
typedef enum _GXTevColorArg GXTevColorArg;
typedef enum _GXTevAlphaArg GXTevAlphaArg;
typedef enum _GXTevBias GXTevBias;
typedef enum _GXTevClampMode GXTevClampMode;
typedef enum _GXTevKColorID GXTevKColorID;
typedef enum _GXTevKColorSel GXTevKColorSel;
typedef enum _GXTevKAlphaSel GXTevKAlphaSel;
typedef enum _GXTevSwapSel GXTevSwapSel;
typedef enum _GXTevColorChan GXTevColorChan;
typedef enum _GXAlphaOp GXAlphaOp;
typedef enum _GXTevScale GXTevScale;
typedef enum _GXFogType GXFogType;
typedef enum _GXBlendMode GXBlendMode;
typedef enum _GXBlendFactor GXBlendFactor;
typedef enum _GXCompare GXCompare;
typedef enum _GXLogicOp GXLogicOp;
typedef enum _GXPixelFmt GXPixelFmt;
typedef enum _GXZFmt16 GXZFmt16;
typedef enum _GXTevMode GXTevMode;
typedef enum _GXGamma GXGamma;
typedef enum _GXProjectionType GXProjectionType;
typedef enum _GXEvent GXEvent;
typedef enum _GXFBClamp GXFBClamp;
typedef enum _GXAnisotropy GXAnisotropy;
typedef enum _GXZTexOp GXZTexOp;
typedef enum _GXAlphaReadMode GXAlphaReadMode;
typedef enum _GXPerf0 GXPerf0;
typedef enum _GXPerf1 GXPerf1;
typedef enum _GXVCachePerf GXVCachePerf;
typedef enum _GXCopyMode GXCopyMode;
typedef enum _GXMiscToken GXMiscToken;
typedef enum _GXXFFlushVal GXXFFlushVal;
typedef void * VIRetraceCallback; // Funktionszeiger
typedef struct _GXColor GXColor;
typedef struct _GXColorS10 GXColorS10;
typedef struct _GXTexObj GXTexObj;
typedef struct _GXTlutObj GXTlutObj;
typedef struct _GXLightObj GXLightObj;
typedef struct _GXVtxAttrFmtList GXVtxAttrFmtList;
typedef struct _GXTexRegion GXTexRegion;
typedef struct _GXTlutRegion GXTlutRegion;
typedef struct _GXVtxDescList GXVtxDescList;
typedef struct _GXRenderModeObj GXRenderModeObj;
typedef struct _GXFogAdjTable GXFogAdjTable;
typedef long long OSTime;
typedef unsigned int OSTick;
typedef int OSHeapHandle;
typedef void * OSAllocVisitor; // Funktionszeiger
typedef void * OSAlarmHandler; // Funktionszeiger
typedef unsigned char __OSException;
typedef void * __OSExceptionHandler; // Funktionszeiger
typedef unsigned short OSError;
typedef void * OSErrorHandler; // Funktionszeiger
typedef short __OSInterrupt;
typedef void * __OSInterruptHandler; // Funktionszeiger
typedef unsigned int OSInterruptMask;
typedef int OSPriority;
typedef void * OSIdleFunction; // Funktionszeiger
typedef void * OSSwitchThreadCallback; // Funktionszeiger
typedef void * OSMessage;
typedef unsigned int OSModuleID;
typedef void * OSResetCallback; // Funktionszeiger
typedef void * OSResetFunction; // Funktionszeiger
typedef Vec * VecPtr;
typedef Vec Point3d;
typedef Vec * Point3dPtr;
typedef S16Vec * S16VecPtr;
typedef Quaternion * QuaternionPtr;
typedef Quaternion Qtrn;
typedef Quaternion * QtrnPtr;
typedef float Mtx[3][4];
typedef void * MtxPtr; // Funktionszeiger
typedef float ROMtx[4][3];
typedef void * ROMtxPtr; // Funktionszeiger
typedef float Mtx44[4][4];
typedef void * Mtx44Ptr; // Funktionszeiger
typedef MtxStack * MtxStackPtr;
typedef void * GXTexRegionCallback; // Funktionszeiger
typedef void * GXTlutRegionCallback; // Funktionszeiger
typedef void * eVERTEXBUFFER;
typedef void * eINDEXBUFFER;
typedef void * GXBreakPtCallback; // Funktionszeiger
typedef void * GXDrawSyncCallback; // Funktionszeiger
typedef void * GXDrawDoneCallback; // Funktionszeiger
typedef union uPPCWGPipe PPCWGPipe;
typedef void * GXVerifyCallback; // Funktionszeiger
typedef double sn_ps;
typedef int __int32_t;
typedef unsigned int __uint32_t;
typedef unsigned int size_t;
typedef unsigned int clock_t;
typedef int time_t;
typedef unsigned int ULong;
typedef int _fpos_t;
typedef struct __fmath_type fBaseType;
typedef void * PADSamplingCallback; // Funktionszeiger
typedef void * JoyHandler; // Funktionszeiger
typedef void * JoyHandlerAlternate; // Funktionszeiger
typedef void * JoyScanner; // Funktionszeiger
typedef void * DVDCBCallback; // Funktionszeiger
typedef void * DVDCallback; // Funktionszeiger
typedef void * AISCallback; // Funktionszeiger
typedef void * AIDCallback; // Funktionszeiger
typedef void * ARCallback; // Funktionszeiger
typedef struct _AXPROFILE AXPROFILE;
typedef struct _AXPBMIX AXPBMIX;
typedef struct _AXPBITD AXPBITD;
typedef struct _AXPBUPDATE AXPBUPDATE;
typedef struct _AXPBDPOP AXPBDPOP;
typedef struct _AXPBVE AXPBVE;
typedef struct _AXPBFIR AXPBFIR;
typedef struct _AXPBADDR AXPBADDR;
typedef struct _AXPBADPCM AXPBADPCM;
typedef struct _AXPBSRC AXPBSRC;
typedef struct _AXPBADPCMLOOP AXPBADPCMLOOP;
typedef struct _AXPBLPF AXPBLPF;
typedef struct _AXPB AXPB;
typedef struct _AXSPB AXSPB;
typedef void * AXUserCallback; // Funktionszeiger
typedef void * AXAuxCallback; // Funktionszeiger
typedef void * AXVoiceCallback; // Funktionszeiger
typedef struct _AXVPB AXVPB;
typedef struct _AXPBU AXPBU;
typedef struct _AXPBITDBUFFER AXPBITDBUFFER;
typedef void * ARQCallback; // Funktionszeiger
typedef void * DSPCallback; // Funktionszeiger
typedef struct STRUCT_DSP_TASK DSPTaskInfo;
typedef void * CARDCallback; // Funktionszeiger
typedef DSLink * DSLinkPtr;
typedef DSList * DSListPtr;
typedef DSBranch * DSBranchPtr;
typedef DSTree * DSTreePtr;
typedef unsigned short DSHashFunc(char *);
typedef DSCacheNode * DSCacheNodePtr;
typedef DSCache * DSCachePtr;
typedef CLUTHeader * CLUTHeaderPtr;
typedef TEXHeader * TEXHeaderPtr;
typedef TEXDescriptor * TEXDescriptorPtr;
typedef TEXPalette * TEXPalettePtr;
typedef int fpos_t;
typedef struct __sFILE FILE;
typedef void * __FileSysCallback; // Funktionszeiger
typedef char INT8;
typedef unsigned char UINT8;
typedef short INT16;
typedef unsigned short UINT16;
typedef int INT32;
typedef unsigned int UINT32;
typedef long long INT64;
typedef unsigned long long UINT64;
typedef void * MemVectorAlloc; // Funktionszeiger
typedef void * MemVectorFree; // Funktionszeiger
typedef void * MemVectorFreeSize; // Funktionszeiger
typedef void THREADPROC();
typedef void THREAD2PROC(void *);
typedef int SYNCTASKFUNC(int, int);
typedef unsigned int QUEUEFUNC(REALELEMENT *, void *);
typedef int QUEUECOMPAREFUNC(REALELEMENT *, REALELEMENT *);
typedef void PRINTFUNCTION(PRINTCHANNEL, const char *);
typedef SYNCTASKFUNC EACHOOKCALLBACKFUNC;
typedef unsigned short USTR;
typedef long long FILE_SIZE_T;
typedef int FILEOP;
typedef void FILESYS_CALLBACK(int, int, void *);
typedef int FILESYS_ATOM(int, void *);
typedef void ASYNCFILE_CALLBACK(int);
typedef int STREAMHANDLE;
typedef int STREAMREQUESTID;
typedef struct MemBlockInfo_t MemBlockInfo;
typedef void * AllocFreeCallback; // Funktionszeiger
typedef unsigned int ARAM_POINTER;
typedef void MOVER(unsigned char *, const unsigned char *, int);
typedef void bFunkHandleASyncFunction(const void *, int);
typedef int bFunkHandleSyncFunction(const void *, int, void *);
typedef FunctionClient MemberDataClient;
typedef FunctionClient GlobalVariableClient;
typedef int ptrdiff_t;
typedef unsigned int wint_t;
typedef char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef char int_least8_t;
typedef short int_least16_t;
typedef int int_least32_t;
typedef unsigned char uint_least8_t;
typedef unsigned short uint_least16_t;
typedef unsigned int uint_least32_t;
typedef char int_fast8_t;
typedef short int_fast16_t;
typedef int int_fast32_t;
typedef unsigned char uint_fast8_t;
typedef unsigned short uint_fast16_t;
typedef unsigned int uint_fast32_t;
typedef float float_t;
typedef double double_t;
typedef long long int64_t;
typedef unsigned long long uint64_t;
typedef unsigned long long intptr_t;
typedef long long intmax_t;
typedef unsigned long long uintmax_t;
typedef void * SceneryCallback; // Funktionszeiger
typedef FogDistDef * FogDistDefPtr;
typedef FogVolumeDef * FogVolumeDefPtr;
typedef unsigned int hObj;
typedef unsigned int hAtt;
typedef unsigned int hLayer;
typedef void * MatrixMultiplier; // Funktionszeiger
typedef unsigned short CheckSum;
typedef void * NewFunctionType; // Funktionszeiger
typedef void * DeleteFunctionType; // Funktionszeiger
typedef int Int4[4];
typedef short Short4[4];
typedef unsigned int argb8888;
typedef void * Constructor; // Funktionszeiger
typedef void * Destructor; // Funktionszeiger
typedef void * RuntimeAllocConstructor; // Funktionszeiger
typedef void * RuntimeAllocDestructor; // Funktionszeiger
typedef void * DynamicUserCallback; // Funktionszeiger
typedef int i32;
typedef short i16;
typedef char i8;
typedef void * MenuScreenCreateFunction; // Funktionszeiger
typedef int fixed;
typedef COORD4 RQUAT;
typedef unsigned int RCROT;
typedef struct HANDLE_def HANDLE;
typedef struct sepdef sep;
typedef union OP_def OP;
typedef struct FILEOPERATION_def FILEOPERATION;
typedef struct FILEBIG_def FILEBIG;
typedef enum MemFill_t MemFill;
typedef struct GFileDevice_def GFileDevice;
typedef int asyncopfunc(int, int, void *, int, int, void *);
typedef struct REQUESTSTRUCTtag REQUESTSTRUCT;
typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;
typedef unsigned int u_long;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef int daddr_t;
typedef char * caddr_t;
typedef unsigned short ino_t;
typedef short dev_t;
typedef int off_t;
typedef unsigned short uid_t;
typedef unsigned short gid_t;
typedef int pid_t;
typedef int key_t;
typedef int ssize_t;
typedef unsigned int mode_t;
typedef unsigned short nlink_t;
typedef int fd_mask;

// ---- Strukturen (Abhängigkeitsreihenfolge) ----
// size 0x8
struct ARAM_DLList {
    ARAM_DLNode *head; // +0x0
    ARAM_DLNode *tail; // +0x4
};
// size 0x8
struct ARAM_DLNode {
    struct ARAM_DLNode *next; // +0x0
    struct ARAM_DLNode *prev; // +0x4
};
// size 0x1C
struct ARAM_POOL {
    ARAM_DLNode MyNode; // +0x0
    ARAM_DLList FreeList; // +0x8
    ARAM_DLList AllocList; // +0x10
    char *Name; // +0x18
};
// size 0x14
struct ARAM_POOL_MANAGER {
    ARAM_DLList PoolList; // +0x0
    unsigned int Alignment; // +0x8
    unsigned int MaxSizeOfDebugString; // +0xC
    void *NodePool; // +0x10
};
// size 0x4
struct ARGB {
    unsigned char a; // +0x0
    unsigned char r; // +0x1
    unsigned char g; // +0x2
    unsigned char b; // +0x3
};
// size 0x20
struct ARQRequest {
    struct ARQRequest *next; // +0x0
    unsigned int owner; // +0x4
    unsigned int type; // +0x8
    unsigned int priority; // +0xC
    unsigned int source; // +0x10
    unsigned int dest; // +0x14
    unsigned int length; // +0x18
    void (* callback)(unsigned long); // +0x1C
};
// size 0x8
struct ASinTableEntry {
    unsigned short Angle; // +0x0
    char _pad0[0x2]; // +0x2
    float Slope; // +0x4
};
// size 0x1
struct AcidActiveGroup {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x1
struct AcidEffect {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x8
struct bNode {
    struct bNode *Next; // +0x0
    struct bNode *Prev; // +0x4
};
// size 0x8
struct bList {
    struct bNode HeadNode; // +0x0
};
// size 0x8
struct bTList_AcidActiveGroup_ {
    struct bList base; // +0x0  Basisklasse
};
// size 0x8
struct bTList_AcidEmitter_ {
    struct bList base; // +0x0  Basisklasse
};
// size 0x10
struct bVector3 {
    float x; // +0x0
    float y; // +0x4
    float z; // +0x8
    float pad; // +0xC
};
// size 0x60
struct AcidEffectSystem {
    struct bTList_AcidActiveGroup_ ActiveGroupList; // +0x0
    struct bTList_AcidEmitter_ AutoEmitterList; // +0x8
    struct bVector3 InterestPoints[3]; // +0x10
    struct AcidEmitter *CurrentEmitter; // +0x40
    int SelectEmitterIndex; // +0x44
    char DebugStats; // +0x48
    char CreateEmitterMaxScript; // +0x49
    char pad; // +0x4A
    char PrintEmitterList; // +0x4B
    char PrintNearbyEmitters; // +0x4C
    char OnlyShowCurrentEmitter; // +0x4D
    char _pad0[0x2]; // +0x4E
    int NumAutoEmitters; // +0x50
    int NumVisibleParticles; // +0x54
    float RenderTime; // +0x58
    float UpdateTime; // +0x5C
};
// size 0x8
struct bTNode_AcidEmitter_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0x10
struct bVector4 {
    float x; // +0x0
    float y; // +0x4
    float z; // +0x8
    float w; // +0xC
};
// size 0x40
struct bMatrix4 {
    struct bVector4 v0; // +0x0
    struct bVector4 v1; // +0x10
    struct bVector4 v2; // +0x20
    struct bVector4 v3; // +0x30
};
// size 0xC0
struct AcidEmitter {
    struct bTNode_AcidEmitter_ base; // +0x0  Basisklasse
    unsigned int NameHash; // +0x8
    unsigned int EmitterDataNameHash; // +0xC
    int State; // +0x10
    float RemainingCycleTime; // +0x14
    float EmissionCountdown; // +0x18
    struct AcidEmitterData *EmitterData; // +0x1C
    struct AcidActiveGroup *LastActiveGroup; // +0x20
    unsigned int RandomSeed; // +0x24
    unsigned int DontEmit; // +0x28
    unsigned int pad2; // +0x2C
    struct bMatrix4 LocalWorld; // +0x30
    struct bVector4 InheritVelocity; // +0x70
    char Name[64]; // +0x80
};
// size 0x8
struct bTNode_AcidEmitterData_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0x220
struct AcidEmitterData {
    struct bTNode_AcidEmitterData_ base; // +0x0  Basisklasse
    unsigned int NameHash; // +0x8
    unsigned int TextureNameHash; // +0xC
    unsigned int GroupNameHash; // +0x10
    struct TextureInfo *Texture; // +0x14
    char SpreadAsDisc; // +0x18
    char ContactSheetW; // +0x19
    char ContactSheetH; // +0x1A
    char AnimFPS; // +0x1B
    char RandomStartFrame; // +0x1C
    char RandomRotationDirection; // +0x1D
    char MotionLive; // +0x1E
    char pad1; // +0x1F
    float OnCycle; // +0x20
    float OnCycleVariance; // +0x24
    float OffCycle; // +0x28
    float OffCycleVariance; // +0x2C
    float NumParticles; // +0x30
    float NumParticlesVariance; // +0x34
    float Life; // +0x38
    float LifeVariance; // +0x3C
    float Speed; // +0x40
    float SpeedVariance; // +0x44
    float InitialAngleRange; // +0x48
    float SpreadAngle; // +0x4C
    float MotionInherit; // +0x50
    float MotionInheritVariance; // +0x54
    enum CarEffectPosition CarPosition; // +0x58
    int pad2; // +0x5C
    struct bVector4 VolumeCenter; // +0x60
    struct bVector4 VolumeExtent; // +0x70
    float FarClip; // +0x80
    float Gravity; // +0x84
    float Drag; // +0x88
    float MaxPixelSize; // +0x8C
    struct bVector4 KeyPositions; // +0x90
    struct bMatrix4 ColourMatrix; // +0xA0
    struct bVector4 Size; // +0xE0
    struct bVector4 RelativeAngle; // +0xF0
    struct bMatrix4 ColourBasis; // +0x100
    struct bMatrix4 ExtraBasis; // +0x140
    char Name[64]; // +0x180
    char TextureName[48]; // +0x1C0
    char GroupName[48]; // +0x1F0
};
// size 0x10
struct AcidEmitterDataPackHeader {
    int pad0; // +0x0
    int pad1; // +0x4
    int Version; // +0x8
    int NumEmitterDatas; // +0xC
};
// size 0x10
struct AcidEmitterPackHeader {
    int pad0; // +0x0
    int pad1; // +0x4
    int Version; // +0x8
    int NumEmitters; // +0xC
};
// size 0x10
struct AeroInfo {
    float Drag; // +0x0
    float FrontDownforce; // +0x4
    float RearDownforce; // +0x8
    float AutoBalanceDownforce; // +0xC
};
// size 0x8
struct bTNode_AllocationHeader_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0x18
struct AllocationHeader {
    struct bTNode_AllocationHeader_ base; // +0x0  Basisklasse
    unsigned char PoolNum; // +0x8
    unsigned char FrontPadding; // +0x9
    short MagicNumber; // +0xA
    int Size; // +0xC
    int AllocationNumber; // +0x10
    char DebugLine; // +0x14
    char DebugText[3]; // +0x15
};
// size 0x18
struct AnimBank {
    int mSize; // +0x0
    int mNumAnims; // +0x4
    struct AttributeDictionary *mAttributeDictionary; // +0x8
    struct AnimMemoryMap **mAnims; // +0xC
    const char **mNames; // +0x10
    struct BankStat *mStat; // +0x14
};
// size 0x2
struct AnimTypeId {
    unsigned short mId; // +0x0
};
// size 0x4
struct AnimMemoryMap {
    struct AnimTypeId mAnimTypeId; // +0x0
    unsigned short mTargetCheckSum; // +0x2
};
// size 0x1
struct AnimStat {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x8
struct bTNode_Mover_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0x14
struct Mover {
    void **vptr; // +0x0  Vptr (MSVC: am Anfang)
    struct bTNode_Mover_ base; // +0x4  Basisklasse
    enum eMovementPhase mMovementPhase; // +0xC
    struct SpaceNode *mpSpaceNode; // +0x10
};
// size 0x6C (GC: 0x74)
struct AnimationMover {
    struct Mover base; // +0x0  Basisklasse
    struct CAnimCtrl *mpAnimCtrl; // +0x14
    struct bMatrix4 BaseMatrix; // +0x18
    float mEstimatedTimeUntilTriggered; // +0x58
    float mTriggerDAT; // +0x5C
    struct WorldModel *mpTriggerModel; // +0x60
    float mPreviousSkeletalAnimationElevation; // +0x64
    bool mRenderTrigger; // +0x68
    bool mKeepSkeletalAnimationOnGround; // +0x69
    bool mHavePreviousSkeletalAnimationElevation; // +0x6A
    char _pad0[0x1]; // +0x6B
};
// size 0x4
struct AnimationTarget {
    unsigned short mCheckSum; // +0x0
    unsigned short mNumDofs; // +0x2
};
// size 0x2
struct AttributeId {
    unsigned short mId; // +0x0
};
// size 0x8
struct Attribute {
    struct AttributeId mId; // +0x0
    unsigned short mSize; // +0x2
    void *mData; // +0x4
};
// size 0xC
struct AttributeBlock {
    int mNumAttributes; // +0x0
    struct Attribute mAttributes[1]; // +0x4
};
// size 0x1
struct AttributeDictionary {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x8
struct AverageBase {
    void **vptr; // +0x0  Vptr (MSVC: am Anfang)
    unsigned char nSize; // +0x4
    unsigned char nSlots; // +0x5
    unsigned char nSamples; // +0x6
    unsigned char nCurrentSlot; // +0x7
};
// size 0x28
struct Average {
    struct AverageBase base; // +0x0  Basisklasse
    float fTotal; // +0x8
    float fAverage; // +0xC
    float *pData; // +0x10
    float SmallDataBuffer[5]; // +0x14
};
// size 0x34
struct AverageWindow {
    struct Average base; // +0x0  Basisklasse
    float fTimeWindow; // +0x28
    int iOldestValue; // +0x2C
    float *pTimeData; // +0x30
};
// size 0x1
struct Avoidable {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x5
struct BIGENTRY16 {
    char offset[2]; // +0x0
    char length[2]; // +0x2
    char name[1]; // +0x4
};
// size 0x7
struct BIGENTRY24 {
    char offset[3]; // +0x0
    char length[3]; // +0x3
    char name[1]; // +0x6
};
// size 0x9
struct BIGENTRY32 {
    char offset[4]; // +0x0
    char length[4]; // +0x4
    char name[1]; // +0x8
};
// size 0xA
struct BIGENTRY40 {
    char offset[5]; // +0x0
    char length[4]; // +0x5
    char name[1]; // +0x9
};
// size 0x8
struct BIGFileInfo {
    struct FILEBIG_def *fi; // +0x0
    int bigoffset; // +0x4
};
// size 0x10
struct BIGHEADER {
    char type[4]; // +0x0
    char len[4]; // +0x4
    char num[4]; // +0x8
    char hlen[4]; // +0xC
};
// size 0x6
struct BIGHEADER24 {
    char type[2]; // +0x0
    char hlen[2]; // +0x2
    char num[2]; // +0x4
};
// size 0x1
struct BankStat {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x1
struct Base64 {
    char _empty; // +0x0 (leere Klasse)
};
// size 0xB4
struct BodyTypeInfo {
    char BodyTypeName[24]; // +0x0
    unsigned int ObjectNameHashA; // +0x18
    unsigned int ObjectNameHashB; // +0x1C
    unsigned int ObjectNameHashC; // +0x20
    unsigned int MappedBodyHash; // +0x24
    unsigned int MappedHeadHash; // +0x28
    unsigned int NumBodyHash; // +0x2C
    unsigned int NumHeadHash; // +0x30
    unsigned int BodySkinHash[16]; // +0x34
    unsigned int HeadSkinHash[16]; // +0x74
};
// size 0x20
struct BoneMask {
    unsigned int mMask[8]; // +0x0
};
// size 0x10
struct BrakeUpgradeInfo {
    float Brake; // +0x0
    float EBrake; // +0x4
    float BrakeBias; // +0x8
    float pad1; // +0xC
};
// size 0x14
struct CARDFileInfo {
    int chan; // +0x0
    int fileNo; // +0x4
    int offset; // +0x8
    int length; // +0xC
    unsigned short iBlock; // +0x10
    unsigned short __padding; // +0x12
};
// size 0x6C
struct CARDStat {
    char fileName[32]; // +0x0
    unsigned int length; // +0x20
    unsigned int time; // +0x24
    unsigned char gameName[4]; // +0x28
    unsigned char company[2]; // +0x2C
    unsigned char bannerFormat; // +0x2E
    unsigned char __padding; // +0x2F
    unsigned int iconAddr; // +0x30
    unsigned short iconFormat; // +0x34
    unsigned short iconSpeed; // +0x36
    unsigned int commentAddr; // +0x38
    unsigned int offsetBanner; // +0x3C
    unsigned int offsetBannerTlut; // +0x40
    unsigned long offsetIcon[8]; // +0x44
    unsigned int offsetIconTlut; // +0x64
    unsigned int offsetData; // +0x68
};
// size 0x1
struct CAnimCtrl {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x8
struct bTNode_CAnimSkeleton_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0x24
struct CAnimSkeleton {
    struct bTNode_CAnimSkeleton_ base; // +0x0  Basisklasse
    int m_loaded; // +0x8
    const char *m_pName; // +0xC
    unsigned int m_NameHash; // +0x10
    int m_internalDynLoader; // +0x14
    struct DynamicLoader *m_pDynLoader; // +0x18
    int m_DynLoaderSize; // +0x1C
    struct Skeleton *m_pSkeleton; // +0x20
};
// size 0xC
struct CLUTHeader {
    unsigned short numEntries; // +0x0
    unsigned char unpacked; // +0x2
    unsigned char pad8; // +0x3
    GXTlutFmt format; // +0x4
    char *data; // +0x8
};
// size 0x10
struct CONVERTER {
    MOVER *mover; // +0x0
    unsigned int destType; // +0x4
    bool swizzleSupport; // +0x8
    char _pad0[0x3]; // +0x9
    unsigned int chunkSize; // +0xC
};
// size 0x8
struct COORD2 {
    float x; // +0x0
    float y; // +0x4
};
// size 0x4
struct COORD2_s16 {
    short x; // +0x0
    short y; // +0x2
};
// size 0x2
struct COORD2_s8 {
    char x; // +0x0
    char y; // +0x1
};
// size 0x4
struct COORD2_u16 {
    unsigned short x; // +0x0
    unsigned short y; // +0x2
};
// size 0x2
struct COORD2_u8 {
    unsigned char x; // +0x0
    unsigned char y; // +0x1
};
// size 0xC
struct COORD3 {
    float x; // +0x0
    float y; // +0x4
    float z; // +0x8
};
// size 0x18
struct COORD32 {
    float x1; // +0x0
    float y1; // +0x4
    float z1; // +0x8
    float x2; // +0xC
    float y2; // +0x10
    float z2; // +0x14
};
// size 0x24
struct COORD33 {
    float x1; // +0x0
    float y1; // +0x4
    float z1; // +0x8
    float x2; // +0xC
    float y2; // +0x10
    float z2; // +0x14
    float x3; // +0x18
    float y3; // +0x1C
    float z3; // +0x20
};
// size 0x6
struct COORD3_s16 {
    short x; // +0x0
    short y; // +0x2
    short z; // +0x4
};
// size 0x3
struct COORD3_s8 {
    char x; // +0x0
    char y; // +0x1
    char z; // +0x2
};
// size 0x6
struct COORD3_u16 {
    unsigned short x; // +0x0
    unsigned short y; // +0x2
    unsigned short z; // +0x4
};
// size 0x3
struct COORD3_u8 {
    unsigned char x; // +0x0
    unsigned char y; // +0x1
    unsigned char z; // +0x2
};
// size 0x10
struct COORD4 {
    float x; // +0x0
    float y; // +0x4
    float z; // +0x8
    float w; // +0xC
};
// size 0x18
struct CPUINFO {
    unsigned long long hz; // +0x0
    char name[16]; // +0x8
};
// size 0x8
struct bTNode_CachedRealFileHandle_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0xDC
struct CachedRealFileHandle {
    struct bTNode_CachedRealFileHandle_ base; // +0x0  Basisklasse
    int NumReferences; // +0x8
    int FileHandle; // +0xC
    int FileSize; // +0x10
    char Filename[200]; // +0x14
};
// size 0x10
struct bVector3Image {
    struct bVector3 base; // +0x0  Basisklasse
};
// size 0xCC
struct CameraParams {
    struct bMatrix4 Matrix; // +0x0
    struct bVector3Image Position; // +0x40
    struct bVector3Image Direction; // +0x50
    struct bVector3Image Target; // +0x60
    struct bVector4 NoiseFrequency1; // +0x70
    struct bVector4 NoiseAmplitude1; // +0x80
    struct bVector4 NoiseFrequency2; // +0x90
    struct bVector4 NoiseAmplitude2; // +0xA0
    float TargetDistance; // +0xB0
    float FocalDistance; // +0xB4
    float DepthOfField; // +0xB8
    float NearZ; // +0xBC
    float FarZ; // +0xC0
    unsigned short FieldOfView; // +0xC4
    unsigned short DummyAngle; // +0xC6
    float DummyFloat; // +0xC8
};
// size 0x27C
struct Camera {
    struct CameraParams CurrentKey; // +0x0
    struct CameraParams PreviousKey; // +0xCC
    struct CameraParams VelocityKey; // +0x198
    bool bClearVelocity; // +0x264
    char _pad0[0x3]; // +0x265
    float ElapsedTime; // +0x268
    int LastUpdateTime; // +0x26C
    int LastDisparateTime; // +0x270
    struct Car *CarNotToRender; // +0x274
    int RenderDash; // +0x278
};
// size 0x1
struct CameraDataItems {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x1
struct CameraMover {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x4C
struct CameraNoiseParams {
    float Frequency1[4]; // +0x0
    float Frequency2[4]; // +0x10
    float Amplitude1[4]; // +0x20
    float Amplitude2[4]; // +0x30
    float ScaleAll; // +0x40
    float ScaleX; // +0x44
    float ScaleY; // +0x48
};
// size 0x1
struct Car {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x30
struct WheelInfo {
    struct bVector3Image Position; // +0x0
    float Radius; // +0x10
    float Width; // +0x14
    int WheelNumber; // +0x18
    float YRenderOffset; // +0x1C
    float WideBodyYRenderOffset; // +0x20
    float pad[3]; // +0x24
};
// size 0x20
struct TireInfo {
    float MaxSlipAngle; // +0x0
    float MaxLoad; // +0x4
    float ForceScale; // +0x8
    float StaticFrictionCoefficient; // +0xC
    float DynamicFrictionCoefficient; // +0x10
    float dummy[3]; // +0x14
};
// size 0x60
struct RigidBodyInfo {
    float Mass; // +0x0
    float Xdim; // +0x4
    float Ydim; // +0x8
    float Zdim; // +0xC
    struct bMatrix4 InertiaTensor; // +0x10
    float InertiaTensorZExtend; // +0x50
    float pad[3]; // +0x54
};
// size 0x20
struct SuspensionInfo {
    float Progression; // +0x0
    float Stiffness; // +0x4
    float Damping[2]; // +0x8
    float SwayBarStiffness; // +0x10
    float BumpStop[2]; // +0x14
    float MaxRenderingCompression; // +0x1C
};
// size 0x40
struct DriveTrainInfo {
    float ShiftUpPoint; // +0x0
    float ShiftDownRPM; // +0x4
    float FinalDrive[2]; // +0x8
    float TorqueSplit; // +0x10
    float ClutchStiffness; // +0x14
    int NumGears; // +0x18
    float ShiftSpeed; // +0x1C
    float GearRatios[8]; // +0x20
};
// size 0x70
struct EngineInfo {
    float IdleRPM; // +0x0
    float RedLineRPM; // +0x4
    float MaxRPM; // +0x8
    float MomentOfInertia; // +0xC
    float TorqueCurveTable[9]; // +0x10
    float TurboTorqueCurve[9]; // +0x34
    float EngineBrakingTable[3]; // +0x58
    float pad1; // +0x64
    float pad2; // +0x68
    float pad3; // +0x6C
};
// size 0x30
struct ControlInfo {
    float Steering; // +0x0
    float Brake; // +0x4
    float EBrake; // +0x8
    float BrakeBias; // +0xC
    float YawControl; // +0x10
    float YawControlNFS; // +0x14
    float MaxZMomentAcc; // +0x18
    float MaxZMomentAccExtreme; // +0x1C
    float SpeedLimiter; // +0x20
    float dummy[3]; // +0x24
};
// size 0x10
struct NitrousInfo {
    float TotalNOS; // +0x0
    int NumBursts; // +0x4
    float RechargeTime; // +0x8
    float TorqueAdditive; // +0xC
};
// size 0x2B0
struct CarPhysicsInfo {
    struct bVector3 CenterOfGravity; // +0x0
    struct WheelInfo Wheels[4]; // +0x10
    struct TireInfo Tires[2]; // +0xD0
    struct RigidBodyInfo Body; // +0x110
    struct SuspensionInfo Suspension[2]; // +0x170
    struct DriveTrainInfo DriveTrain; // +0x1B0
    struct EngineInfo Engine; // +0x1F0
    struct AeroInfo Aero; // +0x260
    struct ControlInfo Control; // +0x270
    struct NitrousInfo Nitrous; // +0x2A0
};
// size 0x18
struct CarWorldObjectMover {
    struct Mover base; // +0x0  Basisklasse
    struct Car *mpCar; // +0x14
};
// size 0x44
struct CharacterInfo {
    int CharacterNumber; // +0x0
    char CharacterName[32]; // +0x4
    char BodyTypeName[24]; // +0x24
    int BodySkinNumber; // +0x3C
    int HeadSkinNumber; // +0x40
};
// size 0x8
struct bTNode_Clan_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0x8
struct bTList_bPNode_ {
    struct bList base; // +0x0  Basisklasse
};
// size 0x8
struct bPList_SkidSet_ {
    struct bTList_bPNode_ base; // +0x0  Basisklasse
};
// size 0x48
struct Clan {
    struct bTNode_Clan_ base; // +0x0  Basisklasse
    struct bPList_SkidSet_ SkidSetList; // +0x8
    unsigned int Hash; // +0x10
    int LastUpdateTime; // +0x14
    struct bVector3 Position; // +0x18
    struct bVector3 BBoxMin; // +0x28
    struct bVector3 BBoxMax; // +0x38
};
// size 0x1
struct Class {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x10
struct ClassConstructorClient {
    void *reserved[2]; // +0x0
    void (* pClientFunc)(struct Class *, Parameter *, void *); // +0x8
    void *pClientData; // +0xC
};
// size 0x10
struct ClassDestructorClient {
    void *reserved[2]; // +0x0
    void (* pClientFunc)(struct Class *, void *); // +0x8
    void *pClientData; // +0xC
};
// size 0x8
struct ClassHandle {
    void *mpPrivate; // +0x0
    int mKey; // +0x4
};
// size 0x1
struct Collision {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x1
struct CollisionSample {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x4
struct Colour {
    unsigned int c; // +0x0
};
// size 0x10
struct WeightReductionUpgradeInfo {
    float FractionOfBaseWeight; // +0x0
    float ForwardCOGAdjustment; // +0x4
    float InertiaTensorZExtend; // +0x8
    float pad1; // +0xC
};
// size 0x40
struct TransmissionUpgradeInfo {
    float ShiftUpPoint; // +0x0
    float ShiftDownRPM; // +0x4
    float FinalDrive[2]; // +0x8
    float TorqueSplit; // +0x10
    float ClutchStiffness; // +0x14
    int NumGears; // +0x18
    float ShiftSpeed; // +0x1C
    float GearRatios[8]; // +0x20
};
// size 0x40
struct EngineUpgradeInfo {
    float IdleRPMAdjust; // +0x0
    float RedLineRPMAdjust; // +0x4
    float MaxRPMAdjust; // +0x8
    float MomentOfInertiaAdjust; // +0xC
    float TorqueCurveAdjust[9]; // +0x10
    float EngineBrakingAdjust[3]; // +0x34
};
// size 0x30
struct TurboUpgradeInfo {
    float TorqueAdditive[9]; // +0x0
    float pad1; // +0x24
    float pad2; // +0x28
    float pad3; // +0x2C
};
// size 0x40
struct TireUpgradeInfo {
    struct TireInfo FrontTires; // +0x0
    struct TireInfo RearTires; // +0x20
};
// size 0x10
struct NitrousUpgradeInfo {
    struct NitrousInfo NOS; // +0x0
};
// size 0x10
struct SpoilerUpgradeInfo {
    float FrontDownforceAdjust; // +0x0
    float RearDownforceAdjust; // +0x4
    float AutoBalanceAdjust; // +0x8
    float pad1; // +0xC
};
// size 0x10
struct GroundEffectsUpgradeInfo {
    float FrontDownforceAdjust; // +0x0
    float RearDownforceAdjust; // +0x4
    float AutoBalanceAdjust; // +0x8
    float pad1; // +0xC
};
// size 0x50
struct SuspensionUpgradeInfo {
    struct SuspensionInfo FrontSuspension; // +0x0
    struct SuspensionInfo RearSuspension; // +0x20
    float COGLowering; // +0x40
    float FrontLowering; // +0x44
    float RearLowering; // +0x48
    float pad2; // +0x4C
};
// size 0x90
struct JunkmanUpgradeInfo {
    float WeightReduction; // +0x0
    float DriveTrainTorqueSplit; // +0x4
    float NOSTorqueAdditive; // +0x8
    float BrakeAdditive; // +0xC
    float StaticFrictionAdditive[2]; // +0x10
    float DynamicFrictionAdditive[2]; // +0x18
    float EngineTorqueAdditive[9]; // +0x20
    float ECUTorqueAdditive[9]; // +0x44
    float TurboTorqueAdditive[9]; // +0x68
    float pad; // +0x8C
};
// size 0x8B0
struct CompleatCarPhysicsInfo {
    struct CarPhysicsInfo BasePhysicsInfo; // +0x0
    struct WeightReductionUpgradeInfo WeightReductionUpgrades[3]; // +0x2B0
    struct TransmissionUpgradeInfo TransmissionUpgrades[3]; // +0x2E0
    struct EngineUpgradeInfo EngineUpgrades[3]; // +0x3A0
    struct EngineUpgradeInfo ECUUpgrades[3]; // +0x460
    struct TurboUpgradeInfo TurboUpgrades[3]; // +0x520
    struct TireUpgradeInfo TireUpgrades[3]; // +0x5B0
    struct NitrousUpgradeInfo NitrousUpgrades[3]; // +0x670
    struct SpoilerUpgradeInfo SpoilerUpgrades[3]; // +0x6A0
    struct GroundEffectsUpgradeInfo GroundEffectsUpgrades[3]; // +0x6D0
    struct BrakeUpgradeInfo BrakeUpgrades[3]; // +0x700
    struct SuspensionUpgradeInfo SuspensionUpgrades[3]; // +0x730
    struct JunkmanUpgradeInfo JunkmanUpgrades; // +0x820
};
// size 0x4
struct ComputationIndexPrivateData {
    short mCurrentComputationIndex; // +0x0
    short mMaxComputationIndex; // +0x2
};
// size 0x4
struct ComputationIndex {
    struct ComputationIndexPrivateData mData; // +0x0
};
// size 0xA0 (GC: 0x100)
struct Force {
    bool Playing[4][8]; // +0x0
    unsigned long EffectID[4][8]; // +0x20
};
// size 0xA0 (GC: 0x100)
struct Condition {
    struct Force base; // +0x0  Basisklasse
};
// size 0xA0 (GC: 0x100)
struct Constant {
    struct Force base; // +0x0  Basisklasse
};
// size 0x14
struct SymbolPool {
    unsigned int mTableSize; // +0x0
    unsigned int mTableLength; // +0x4
    struct SymbolEntry **mpSymbolTable; // +0x8
    unsigned int mSymbolTableCRC32; // +0xC
    struct FunctionEntry *mpFunctions; // +0x10
};
// size 0x28
struct ConstructorPool {
    struct SymbolPool conspool; // +0x0
    struct SymbolPool despool; // +0x14
};
// size 0x1
struct Constructors {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x10
struct DBInterface {
    unsigned int bPresent; // +0x0
    unsigned int exceptionMask; // +0x4
    void (* ExceptionDestination)(); // +0x8
    void *exceptionReturn; // +0xC
};
// size 0xC
struct PADStatus {
    unsigned short button; // +0x0
    char stickX; // +0x2
    char stickY; // +0x3
    char substickX; // +0x4
    char substickY; // +0x5
    unsigned char triggerLeft; // +0x6
    unsigned char triggerRight; // +0x7
    unsigned char analogA; // +0x8
    unsigned char analogB; // +0x9
    char err; // +0xA
    char _pad0[0x1]; // +0xB
};
// size 0x1E
struct DEMOPadStatus {
    PADStatus pst; // +0x0
    unsigned short buttonDown; // +0xC
    unsigned short buttonUp; // +0xE
    unsigned short dirs; // +0x10
    unsigned short dirsNew; // +0x12
    unsigned short dirsReleased; // +0x14
    short stickDeltaX; // +0x16
    short stickDeltaY; // +0x18
    short substickDeltaX; // +0x1A
    short substickDeltaY; // +0x1C
};
// size 0x40
struct DEMOStatObj {
    char text[50]; // +0x0
    char _pad0[0x2]; // +0x32
    DEMOStatType stat_type; // +0x34
    unsigned int stat; // +0x38
    unsigned int count; // +0x3C
};
// size 0x10
struct DSBranch {
    char *Prev; // +0x0
    char *Next; // +0x4
    char *Parent; // +0x8
    char *Children; // +0xC
};
// size 0xC
struct DSList {
    unsigned int Offset; // +0x0
    char *Head; // +0x4
    char *Tail; // +0x8
};
// size 0x10
struct DSCache {
    unsigned char PurgeFlag; // +0x0
    char _pad0[0x3]; // +0x1
    DSList CacheNodeList; // +0x4
};
// size 0x8
struct DSLink {
    char *Prev; // +0x0
    char *Next; // +0x4
};
// size 0x18
struct DSCacheNode {
    DSLink Link; // +0x0
    void (* Free)(char * *); // +0x8
    char *Name; // +0xC
    char *Data; // +0x10
    unsigned short ReferenceCount; // +0x14
    char _pad0[0x2]; // +0x16
};
// size 0xC
struct DSHashTable {
    DSList *table; // +0x0
    unsigned short tableSize; // +0x4
    char _pad0[0x2]; // +0x6
    DSHashFunc *hash; // +0x8
};
// size 0x60
struct DSPADPCM {
    unsigned int num_samples; // +0x0
    unsigned int num_adpcm_nibbles; // +0x4
    unsigned int sample_rate; // +0x8
    unsigned short loop_flag; // +0xC
    unsigned short format; // +0xE
    unsigned int sa; // +0x10
    unsigned int ea; // +0x14
    unsigned int ca; // +0x18
    unsigned short coef[16]; // +0x1C
    unsigned short gain; // +0x3C
    unsigned short ps; // +0x3E
    unsigned short yn1; // +0x40
    unsigned short yn2; // +0x42
    unsigned short lps; // +0x44
    unsigned short lyn1; // +0x46
    unsigned short lyn2; // +0x48
    unsigned short pad[11]; // +0x4A
};
// size 0x8
struct DSTree {
    unsigned int Offset; // +0x0
    char *Root; // +0x4
};
// size 0x30
struct DVDCommandBlock {
    struct DVDCommandBlock *next; // +0x0
    struct DVDCommandBlock *prev; // +0x4
    unsigned int command; // +0x8
    int state; // +0xC
    unsigned int offset; // +0x10
    unsigned int length; // +0x14
    void *addr; // +0x18
    unsigned int currTransferSize; // +0x1C
    unsigned int transferredSize; // +0x20
    DVDDiskID *id; // +0x24
    void (* callback)(long, struct DVDCommandBlock *); // +0x28
    void *userData; // +0x2C
};
// size 0xC
struct DVDDir {
    unsigned int entryNum; // +0x0
    unsigned int location; // +0x4
    unsigned int next; // +0x8
};
// size 0xC
struct DVDDirEntry {
    unsigned int entryNum; // +0x0
    int isDir; // +0x4
    char *name; // +0x8
};
// size 0x20
struct DVDDiskID {
    char gameName[4]; // +0x0
    char company[2]; // +0x4
    unsigned char diskNumber; // +0x6
    unsigned char gameVersion; // +0x7
    unsigned char streaming; // +0x8
    unsigned char streamingBufSize; // +0x9
    unsigned char padding[22]; // +0xA
};
// size 0x3C
struct DVDFileInfo {
    DVDCommandBlock cb; // +0x0
    unsigned int startAddr; // +0x30
    unsigned int length; // +0x34
    void (* callback)(long, struct DVDFileInfo *); // +0x38
};
// size 0x8
struct bTNode_DebugTopology_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0x64
struct DebugTopology {
    struct bTNode_DebugTopology_ base; // +0x0  Basisklasse
    struct TopologyTreePolygon *pTopologyTreePolygon; // +0x8
    int FaceHash; // +0xC
    int CreationTime; // +0x10
    struct bVector3 Vertices[4]; // +0x14
    struct bVector3 Normal; // +0x54
};
// size 0x18
struct DefaultAnimBank {
    struct AnimBank base; // +0x0  Basisklasse
};
// size 0x1
struct DemoCameraCoordinator {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x4
struct Timer {
    int PackedTime; // +0x0
};
// size 0x2C (GC: 0x30)
struct DemoDiscManager {
    bool bActive; // +0x0
    char _pad0[0x3]; // +0x1
    int Dummy; // +0x4
    struct Timer StartTime; // +0x8
    struct Timer LastActivityTime; // +0xC
    enum DemoDiscEndReason nDemoDiscEndReason; // +0x10
    int NumDemoLoops; // +0x14
    unsigned short Language; // +0x18
    unsigned short Aspect; // +0x1A
    unsigned short PlayMode; // +0x1C
    unsigned short InactiveTimeout; // +0x1E
    unsigned short TotalTimeout; // +0x20
    unsigned short MediaType; // +0x22
    unsigned short MasterVolumeScale; // +0x24
    unsigned short DirectorySectorNum; // +0x26
    bool bCalledSCEADemoStart; // +0x28
    bool bCalledSCEEDemoStart; // +0x29
    char _pad1[0x2]; // +0x2A
};
// size 0x1
struct DestructorEntry {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x4
struct DeviceExtension {
    struct Device *mpBaseObject; // +0x0
};
// size 0x10
struct DevicePrivate {
    void * (* mpNewOverride)(unsigned int, const char *); // +0x0
    void (* mpDeleteOverride)(void *, unsigned int); // +0x4
    struct RenderContextBase *mFirstRenderContext; // +0x8
    bool mAmInitialized; // +0xC
    char _pad0[0x3]; // +0xD
};
// size 0x14
struct Device {
    struct DeviceExtension EXTENSION_OBJ; // +0x0
    struct DevicePrivate mPrivate; // +0x4
};
// size 0x1
struct DotWall {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x1
struct DriverInfo {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x1
struct DummyFEngNewType {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x1
struct DynamicCollisionBody {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x2C
struct DynamicLoader {
    void *handle; // +0x0
    int nDestructors; // +0x4
    struct DestructorEntry *destructors; // +0x8
    struct RuntimeAllocDestructorEntry *RuntimeAllocDestructors; // +0xC
    char *mpData; // +0x10
    unsigned int mDataLen; // +0x14
    char *mpReloc; // +0x18
    void * (* mSearchCallback)(const char *, bool *); // +0x1C
    int mNumPatchAddresses; // +0x20
    int mMaxPatchAddresses; // +0x24
    unsigned int **mpPatchAddresses32; // +0x28
};
// size 0x3C (GC: 0x40)
struct EasterEggs {
    void **vptr; // +0x0  Vptr (MSVC: am Anfang)
    bool ButtonsEnabled; // +0x4
    bool HaveUnlockMessage; // +0x5
    char _pad0[0x2]; // +0x6
    unsigned int UnlockMessage; // +0x8
    struct EasterEggsData *EasterEggsTable; // +0xC
    unsigned int NumberOfEasterEggs; // +0x10
    enum EasterEggButtons ButtonBuffer[8]; // +0x14
    unsigned int NumberOfCurrentButtons; // +0x34
    unsigned int CurrentStartButton; // +0x38
};
// size 0x34 (GC: 0x38)
struct EasterEggsData {
    enum EasterEggButtons buttons[8]; // +0x0
    unsigned int type; // +0x20
    unsigned int item; // +0x24
    unsigned int group; // +0x28
    unsigned int unlock_message; // +0x2C
    bool unlocked; // +0x30
    bool persistent; // +0x31
    char _pad0[0x2]; // +0x32
};
// size 0xC
struct ElipseTable {
    short *pTable; // +0x0
    int nTableI; // +0x4
    int nTableJ; // +0x8
};
// size 0x1
struct EndingNisCameraCoordinator {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x1
struct EventHandler {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x1
struct EventNode {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x8
struct bTNode_EventSetup_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0x50
struct EventSetup {
    struct bTNode_EventSetup_ base; // +0x0  Basisklasse
    unsigned int EventSetupHash; // +0x8
    int NumMagSetups; // +0xC
    int PossibleMagSetups[16]; // +0x10
};
// size 0x8
struct FEButtonMap {
    struct FEObject **pList; // +0x0
    unsigned long Count; // +0x4
};
// size 0x1
struct FECodeListBox {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x10
struct FEColor {
    int b; // +0x0
    int g; // +0x4
    int r; // +0x8
    int a; // +0xC
};
// size 0x1
struct FEEffect {
    char _empty; // +0x0 (leere Klasse)
};
// size 0xC
struct FEEvent {
    unsigned long EventID; // +0x0
    unsigned long Target; // +0x4
    unsigned long tTime; // +0x8
};
// size 0x8
struct FEEventList {
    int Count; // +0x0
    struct FEEvent *pEvent; // +0x4
};
// size 0x1
struct FEGameInterface {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x10
struct FEGenericVal {
    unsigned long Data[4]; // +0x0
};
// size 0x1
struct FEGroup {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x10
struct FEMinList {
    void **vptr; // +0x0  Vptr (MSVC: am Anfang)
    unsigned int numElements; // +0x4
    struct FEMinNode *head; // +0x8
    struct FEMinNode *tail; // +0xC
};
// size 0x1C
struct FEHashList {
    void **vptr; // +0x0  Vptr (MSVC: am Anfang)
    int TableSize; // +0x4
    struct FEMinList *HashTable; // +0x8
    struct FEMinList NodeList; // +0xC
};
// size 0xC
struct FEMinNode {
    void **vptr; // +0x0  Vptr (MSVC: am Anfang)
    struct FEMinNode *next; // +0x4
    struct FEMinNode *prev; // +0x8
};
// size 0x18
struct FEHashNode {
    struct FEMinNode base; // +0x0  Basisklasse
    char *Name; // +0xC
    unsigned int Hash; // +0x10
    struct FEHashNodePtr *myPtr; // +0x14
};
// size 0x10
struct FEHashNodePtr {
    struct FEMinNode base; // +0x0  Basisklasse
    struct FEHashNode *NodePtr; // +0xC
};
// size 0x1
struct FEImage {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x14
struct FEKeyData {
    int tTime; // +0x0
    struct FEGenericVal Val; // +0x4
};
// size 0x20
struct FEKeyNode {
    struct FEMinNode base; // +0x0  Basisklasse
    int tTime; // +0xC
    struct FEGenericVal Val; // +0x10
};
// size 0x10
struct FERefList {
    void **vptr; // +0x0  Vptr (MSVC: am Anfang)
    bool bIsReference; // +0x4
    char _pad0[0x3]; // +0x5
    char anon1[4]; // +0x8  anonymer Union/Struct, GC-Größe
    struct FEMinNode *tail; // +0xC
};
// size 0x38
struct FEKeyTrack {
    unsigned char ParamType; // +0x0
    unsigned char ParamSize; // +0x1
    unsigned char InterpType; // +0x2
    unsigned char InterpAction; // +0x3
    unsigned int _bits_0x4; // +0x4  Bitfelder Length:24 LongOffset:8
    struct FEKeyNode BaseKey; // +0x8
    struct FERefList DeltaKeys; // +0x28
};
// size 0x4
struct FELabelCallback {
    void **vptr; // +0x0  Vptr (MSVC: am Anfang)
};
// size 0x10
struct FEList {
    struct FEMinList base; // +0x0  Basisklasse
};
// size 0x40
struct FEMatrix4 {
    float m11; // +0x0
    float m12; // +0x4
    float m13; // +0x8
    float m14; // +0xC
    float m21; // +0x10
    float m22; // +0x14
    float m23; // +0x18
    float m24; // +0x1C
    float m31; // +0x20
    float m32; // +0x24
    float m33; // +0x28
    float m34; // +0x2C
    float m41; // +0x30
    float m42; // +0x34
    float m43; // +0x38
    float m44; // +0x3C
};
// size 0x18
struct FEMessageResponse {
    struct FEMinNode base; // +0x0  Basisklasse
    unsigned long MsgID; // +0xC
    unsigned long Count; // +0x10
    struct FEResponse *pResponseList; // +0x14
};
// size 0x1
struct FEModel {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x1
struct FEMovie {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x14
struct FENode {
    struct FEMinNode base; // +0x0  Basisklasse
    char *name; // +0xC
    unsigned int nameHash; // +0x10
};
// size 0xC
struct FEVector3 {
    float x; // +0x0
    float y; // +0x4
    float z; // +0x8
};
// size 0x10
struct FEQuaternion {
    float x; // +0x0
    float y; // +0x4
    float z; // +0x8
    float w; // +0xC
};
// size 0x44
struct FEObjData {
    struct FEColor Col; // +0x0
    struct FEVector3 Pivot; // +0x10
    struct FEVector3 Pos; // +0x1C
    struct FEQuaternion Rot; // +0x28
    struct FEVector3 Size; // +0x38
};
// size 0x58
struct FEObject {
    struct FEMinNode base; // +0x0  Basisklasse
    unsigned long GUID; // +0xC
    unsigned long NameHash; // +0x10
    char *pName; // +0x14
    enum FEObjType Type; // +0x18
    unsigned long Flags; // +0x1C
    unsigned short RenderContext; // +0x20
    unsigned short ResourceIndex; // +0x22
    unsigned long Handle; // +0x24
    unsigned long UserParam; // +0x28
    unsigned char *pData; // +0x2C
    unsigned long DataSize; // +0x30
    struct FEMinList Responses; // +0x34
    struct FEMinList Scripts; // +0x44
    struct FEScript *pCurrentScript; // +0x54
};
// size 0x4
struct FEObjectDestructorCallback {
    void **vptr; // +0x0  Vptr (MSVC: am Anfang)
};
// size 0x1
struct FEPackage {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x8
struct FEPoint {
    float h; // +0x0
    float v; // +0x4
};
// size 0x10
struct FERect {
    float left; // +0x0
    float top; // +0x4
    float right; // +0x8
    float bottom; // +0xC
};
// size 0xC
struct FEResponse {
    unsigned long ResponseID; // +0x0
    unsigned long ResponseParam; // +0x4
    unsigned long ResponseTarget; // +0x8
};
// size 0x34
struct FEScript {
    struct FEMinNode base; // +0x0  Basisklasse
    int Length; // +0xC
    int CurTime; // +0x10
    unsigned long Flags; // +0x14
    struct FEScript *pChainTo; // +0x18
    unsigned long TrackCount; // +0x1C
    struct FEKeyTrack *pTracks; // +0x20
    struct FEEventList Events; // +0x24
    char *pName; // +0x2C
    unsigned long ID; // +0x30
};
// size 0x8
struct FEWideString {
    short *mpsString; // +0x0
    unsigned long mulBufferLength; // +0x4
};
// size 0x74
struct FEString {
    struct FEObject base; // +0x0  Basisklasse
    char *pLabelName; // +0x58
    unsigned long LabelHash; // +0x5C
    struct FEWideString string; // +0x60
    unsigned long Format; // +0x68
    int Leading; // +0x6C
    unsigned long MaxWidth; // +0x70
};
// size 0x8
struct FEVector2 {
    float x; // +0x0
    float y; // +0x4
};
// size 0x10
struct FEVector4 {
    float x; // +0x0
    float y; // +0x4
    float z; // +0x8
    float w; // +0xC
};
// size 0x18
struct FILEBIG_def {
    void *header; // +0x0
    int headersize; // +0x4
    int memflags; // +0x8
    struct HANDLE_def *handle; // +0xC
    struct FILEOPERATION_def *fop; // +0x10
    struct FILEBIG_def *next; // +0x14
};
// size 0x4
struct sepdef {
    unsigned short count; // +0x0
    unsigned char type; // +0x2
    unsigned char index; // +0x3
};
// size 0x4
union OP_def {
    struct sepdef sep; // +0x0
    int id; // +0x0
};
// size 0x30
struct FILEOPERATION_def {
    union OP_def op; // +0x0
    int cancelled; // +0x4
    volatile int status; // +0x8
    enum FILEERR error; // +0xC
    int priority; // +0x10
    void *userdata; // +0x14
    int value; // +0x18
    int amount; // +0x1C
    void *data; // +0x20
    struct HANDLE_def *handle; // +0x24
    void (* callback)(int, int, void *); // +0x28
    struct FILEOPERATION_def *next; // +0x2C
};
// size 0x14
struct FILEREQUEST {
    const char *name; // +0x0
    void *location; // +0x4
    int parm; // +0x8
    int abortflag; // +0xC
    int flags; // +0x10
};
// size 0x14
struct FILESYSOPTS {
    int size; // +0x0
    void * (* malloc)(const char *, int, int); // +0x4
    bool (* free)(void *); // +0x8
    unsigned char *FileSysThreadStack; // +0xC
    unsigned int FileSysThreadStackSize; // +0x10
};
// size 0x80
struct FONT {
    unsigned int signature; // +0x0
    unsigned int size; // +0x4
    unsigned short version; // +0x8
    unsigned short num; // +0xA
    int flags; // +0xC
    signed char centerx; // +0x10
    signed char centery; // +0x11
    unsigned char ascent; // +0x12
    unsigned char descent; // +0x13
    int chartbl; // +0x14
    int kerntbl; // +0x18
    int shape; // +0x1C
    int states[24]; // +0x20
};
// size 0x10
struct FONTCHAR {
    unsigned short index; // +0x0
    unsigned char width; // +0x2
    unsigned char height; // +0x3
    unsigned short u; // +0x4
    unsigned short v; // +0x6
    signed char advancey; // +0x8
    signed char xoffset; // +0x9
    signed char yoffset; // +0xA
    unsigned char numkern; // +0xB
    unsigned short kernindex; // +0xC
    short advancex; // +0xE
};
// size 0xC
struct FONTCHARBUF {
    const FONTCHAR *ch; // +0x0
    float x; // +0x4
    float y; // +0x8
};
// size 0x14
struct FONTDRIVER {
    void (* draw)(FONT *, const FONTCHAR *, float, float); // +0x0
    void (* startdraw)(FONT *); // +0x4
    void (* enddraw)(FONT *); // +0x8
    void (* createfont)(FONT *); // +0xC
    void (* destroyfont)(FONT *); // +0x10
};
// size 0x1C
struct FONTDRIVEREXT {
    int size; // +0x0
    void (* drawmany)(FONT *, struct FONTCHARBUF *, int); // +0x4
    int (* drawlist_getsize)(int); // +0x8
    void (* drawlist_build)(FONT *, struct FONTCHARBUF *, int, void *, bool); // +0xC
    bool (* drawlist_setstate)(void *, int, unsigned int); // +0x10
    bool (* drawlist_setstatef)(void *, int, float); // +0x14
    void (* drawlist_draw)(void *, float, float); // +0x18
};
// size 0x4
struct FONTKERN {
    unsigned short previouscode; // +0x0
    signed char kernvalue; // +0x2
    unsigned char code; // +0x3
};
// size 0x14
struct FS_EVNT_AS_READ_INFO {
    char *name; // +0x0
    int handle; // +0x4
    int ofs; // +0x8
    int nbytes; // +0xC
    void *raddr; // +0x10
};
// size 0x8
struct FS_EVNT_CB_INFO {
    void (* pCbFn)(FS_EVNT, void *, void *); // +0x0
    void *userdata; // +0x4
};
// size 0xC
struct FS_EVNT_CD_READ_INFO {
    char *name; // +0x0
    int sector; // +0x4
    int num_sectors; // +0x8
};
// size 0x8
struct bTNode_FernFlattener_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0x58
struct FernFlattener {
    struct bTNode_FernFlattener_ base; // +0x0  Basisklasse
    struct bMatrix4 OriginalMatrix; // +0x8
    struct SceneryInstance *pSceneryInstance; // +0x48
    int SlotNumber; // +0x4C
    float FlattenRatio; // +0x50
    float LastFlattenTime; // +0x54
};
// size 0x8
struct bTNode_FernShaker_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0x1C
struct FernShaker {
    struct bTNode_FernShaker_ base; // +0x0  Basisklasse
    unsigned int SceneryCode; // +0x8
    int StartTime; // +0xC
    struct SceneryInstance *pSceneryInstance; // +0x10
    struct WorldModel *pWorldModel; // +0x14
    struct FernShakerInfo *pFernShakerInfo; // +0x18
};
// size 0x10
struct FernShakerInfo {
    unsigned int RigidAnimationNameHash; // +0x0
    unsigned int EmitterGroupNameHash; // +0x4
    struct bTList_AcidEmitter_ EmitterList; // +0x8
};
// size 0x10
struct FloatColour {
    float a; // +0x0
    float r; // +0x4
    float g; // +0x8
    float b; // +0xC
};
// size 0xC
struct FloatVector {
    float x; // +0x0
    float y; // +0x4
    float z; // +0x8
};
// size 0x1
struct FlyByCameraCoordinator {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x4
struct FnAnimBank {
    void **vptr; // +0x0  Vptr (MSVC: am Anfang)
};
// size 0x1
struct FnAnimFactory {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x4
struct FnAnimSuper {
    void **vptr; // +0x0  Vptr (MSVC: am Anfang)
};
// size 0x8
struct FnDefaultAnimBank {
    struct FnAnimBank base; // +0x0  Basisklasse
    struct AnimBank *mpAnimBank; // +0x4
};
// size 0x3C
struct FogDistDef {
    unsigned int colour; // +0x0
    float power; // +0x4
    float fogstart; // +0x8
    float radius; // +0xC
    float falloffstart; // +0x10
    struct bVector3 position; // +0x14
    float FogFalloff; // +0x24
    float FogFalloffX; // +0x28
    float FogFalloffY; // +0x2C
    float distance; // +0x30
    unsigned int inFlags; // +0x34
    float effect; // +0x38
};
// size 0x168
struct FogVolumeDef {
    struct eModel *model; // +0x0
    unsigned int colour; // +0x4
    enum FogVolumeBlend blendtype; // +0x8
    enum FogVolumeType fogtype; // +0xC
    enum FogPalleteModes fogpalette_mode; // +0x10
    float power; // +0x14
    struct bMatrix4 fogmatrix; // +0x18
    struct bMatrix4 invmatrix; // +0x58
    struct bMatrix4 ModifierMatrix; // +0x98
    struct bVector3 ModifierPosition; // +0xD8
    struct bVector3 modelAABB[8]; // +0xE8
};
// size 0x24B8
struct FogVolume {
    float FogFalloff; // +0x0
    float FogFalloffX; // +0x4
    float FogFalloffY; // +0x8
    float DistFogStart; // +0xC
    float DistFogPower; // +0x10
    unsigned int DistFogColour; // +0x14
    struct FogDistDef DistantFogs[150]; // +0x18
    struct FogVolumeDef FogVolumes[1]; // +0x2340
    int indecies[1]; // +0x24A8
    int numVols; // +0x24AC
    int numDist; // +0x24B0
    int numHeadlights; // +0x24B4
};
// size 0x8
struct bTNode_Font_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0x428
struct Font {
    struct bTNode_Font_ base; // +0x0  Basisklasse
    struct tTTGlyph *CharacterLookupTable[256]; // +0x8
    struct tTTFontHeader *Header; // +0x408
    struct tTTGlyph *Glyphs; // +0x40C
    struct tTTKerningPair *Kerning; // +0x410
    struct TextureInfo *pTextureInfo; // +0x414
    float t2w; // +0x418
    float t2h; // +0x41C
    float FontHeight; // +0x420
    float FixedWidthCell; // +0x424
};
// size 0x8
struct bTNode_FreeBlock_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0x10
struct FreeBlock {
    struct bTNode_FreeBlock_ base; // +0x0  Basisklasse
    int Size; // +0x8
    int MagicNumber; // +0xC
};
// size 0x1
struct Function {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x10
struct FunctionClient {
    void *reserved[2]; // +0x0
    void (* pClientFunc)(Parameter *, void *); // +0x8
    void *pClientData; // +0xC
};
// size 0xC
struct FunctionEntry {
    void * (* Address)(const char *, bool *); // +0x0
    struct FunctionEntry *Prev; // +0x4
    struct FunctionEntry *Next; // +0x8
};
// size 0x8
struct FunctionHandle {
    void *mpPrivate; // +0x0
    int mKey; // +0x4
};
// size 0x2
struct GCColourRGB565 {
    unsigned short c; // +0x0
};
// size 0x3
struct GCColourRGB8 {
    unsigned char c[3]; // +0x0
};
// size 0x2
struct GCColourRGBA4 {
    unsigned short c; // +0x0
};
// size 0x3
struct GCColourRGBA6 {
    unsigned char c[3]; // +0x0
};
// size 0x28
struct GFileDevice_def {
    int maxfileops; // +0x0
    int maxopen; // +0x4
    int minpriority; // +0x8
    int numopsqueued; // +0xC
    volatile int callbacklevel; // +0x10
    volatile FILEOPERATION *exec; // +0x14
    FILEOPERATION *oplist; // +0x18
    HANDLE *handlelist; // +0x1C
    FILEOPERATION *opqhead; // +0x20
    FILEBIG *bigfilelist; // +0x24
};
// size 0x80
struct GXFifoObj {
    unsigned char pad[128]; // +0x0
};
// size 0x20
struct GameFlowManager {
    void (* pSingleFunction)(int); // +0x0
    int SingleFunctionParam; // +0x4
    const char *pSingleFunctionName; // +0x8
    void (* pLoopingFunction)(); // +0xC
    const char *pLoopingFunctionName; // +0x10
    bool WaitingForCallback; // +0x14
    char _pad0[0x3]; // +0x15
    const char *pCallbackName; // +0x18
    enum GameFlowState CurrentGameFlowState; // +0x1C
};
// size 0x1
struct GlobalVariable {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x8
struct GlobalVariableHandle {
    void *mpPrivate; // +0x0
    int mKey; // +0x4
};
// size 0xB8
struct SceneryCullInfo {
    struct bMatrix4 FacadeMatrix; // +0x0
    struct bMatrix4 ClipMatrix; // +0x40
    struct eView *pView; // +0x80
    int ExcludeFlags; // +0x84
    struct SceneryDrawInfo *pFirstDrawInfo; // +0x88
    struct SceneryDrawInfo *pCurrentDrawInfo; // +0x8C
    struct SceneryDrawInfo *pTopDrawInfo; // +0x90
    struct bVector3 Position; // +0x94
    struct bVector3 Direction; // +0xA4
    float H; // +0xB4
};
// size 0x740
struct GrandSceneryCullInfo {
    struct SceneryCullInfo SceneryCullInfos[10]; // +0x0
    int NumCullInfos; // +0x730
    struct SceneryDrawInfo *pFirstDrawInfo; // +0x734
    struct SceneryDrawInfo *pCurrentDrawInfo; // +0x738
    struct SceneryDrawInfo *pTopDrawInfo; // +0x73C
};
// size 0x8
struct Graph {
    struct bVector2 *Points; // +0x0
    int NumPoints; // +0x4
};
// size 0x1
struct GraphEntry_bVector2_ {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x1
struct GraphEntry_bVector3_ {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x1
struct GraphEntry_bVector4_ {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x1
struct GraphEntry_float_ {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x144
struct HANDLE_def {
    enum FILE_TypeEnum type; // +0x0
    unsigned char allocated; // +0x4
    char name[255]; // +0x5
    int length; // +0x104
    char info[60]; // +0x108  anonymer Union/Struct, GC-Größe
};
// size 0x4
struct IAllocator {
    void **vptr; // +0x0  Vptr (MSVC: am Anfang)
};
// size 0x1
struct InitTerrainType {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x1
struct Initializer {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x8
struct InterfaceId {
    const char *pString; // +0x0
    short systemCrc; // +0x4
    short interfaceCrc; // +0x6
};
// size 0x1
struct IntroNisCameraCoordinator {
    char _empty; // +0x0 (leere Klasse)
};
// size 0xA0
struct Irradiance {
    float coeffs[10][4]; // +0x0
};
// size 0x10
struct JLZHash {
    int Offset; // +0x0
    unsigned char *pData; // +0x4
    struct JLZHash *pPrev; // +0x8
    struct JLZHash *pNext; // +0xC
};
// size 0xC
struct JLZHashPool {
    struct JLZHash *pPool; // +0x0
    struct JLZHash **pHead; // +0x4
    int PoolSize; // +0x8
};
// size 0x44
struct JollyRancherResponsePacket {
    volatile int UseMatrix; // +0x0
    volatile struct bMatrix4 CamMatrix; // +0x4
};
// size 0xA
struct PadData {
    unsigned short DigitalButtons; // +0x0
    unsigned char LTrigger; // +0x2
    unsigned char RTrigger; // +0x3
    unsigned char AnalogLeftX; // +0x4
    unsigned char AnalogLeftY; // +0x5
    unsigned char AnalogRightX; // +0x6
    unsigned char AnalogRightY; // +0x7
    unsigned char Error; // +0x8
    unsigned char Type; // +0x9
};
// size 0x3C
struct JoyData {
    struct PadData ThePadData[4]; // +0x0
    unsigned char Bytes[4]; // +0x28
    int RegularControllerType; // +0x2C
    struct PADStatus padSTATUS; // +0x30
};
// size 0x1
struct Joylog {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x1
struct JoylogBuffer {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x1
struct Joystick {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x8
struct LGConditionForceParams {
    char offset; // +0x0
    unsigned char deadband; // +0x1
    unsigned char saturationNeg; // +0x2
    unsigned char saturationPos; // +0x3
    short coefficientNeg; // +0x4
    short coefficientPos; // +0x6
};
// size 0xC
struct LGEnvelopeParams {
    unsigned int attackTime; // +0x0
    unsigned int fadeTime; // +0x4
    unsigned char attackLevel; // +0x8
    unsigned char fadeLevel; // +0x9
    unsigned char pad[2]; // +0xA
};
// size 0x10
struct LGConstantForceParams {
    short magnitude; // +0x0
    unsigned short direction; // +0x2
    LGEnvelopeParams envelope; // +0x4
};
// size 0x24
struct LGForceEffect {
    unsigned char type; // +0x0
    unsigned char pad[3]; // +0x1
    unsigned int duration; // +0x4
    unsigned int startDelay; // +0x8
    char p[24]; // +0xC  anonymer Union/Struct, GC-Größe
};
// size 0x18
struct LGPeriodicForceParams {
    unsigned char magnitude; // +0x0
    unsigned char pad; // +0x1
    unsigned short direction; // +0x2
    unsigned short period; // +0x4
    unsigned short phase; // +0x6
    short offset; // +0x8
    unsigned char pad2[2]; // +0xA
    LGEnvelopeParams envelope; // +0xC
};
// size 0xA
struct LGPosition {
    unsigned short button; // +0x0
    unsigned char misc; // +0x2
    char wheel; // +0x3
    unsigned char accelerator; // +0x4
    unsigned char brake; // +0x5
    char combined; // +0x6
    unsigned char triggerLeft; // +0x7
    unsigned char triggerRight; // +0x8
    char err; // +0x9
};
// size 0x6
struct LGRampForceParams {
    short magnitudeStart; // +0x0
    short magnitudeEnd; // +0x2
    unsigned short direction; // +0x4
};
// size 0x880
struct Wheels {
    LGPosition Position[4]; // +0x0
    short NonLinearWheel[4][256]; // +0x28
    unsigned long WheelHandles[4]; // +0x828
    unsigned long type[4]; // +0x838
    unsigned long WheelHandle[4]; // +0x848
    LGPosition PositionLast[4]; // +0x858
};
// size 0xA0 (GC: 0x100)
struct Periodic {
    struct Force base; // +0x0  Basisklasse
};
// size 0xA0 (GC: 0x100)
struct Ramp {
    struct Force base; // +0x0  Basisklasse
};
// size 0x1468 (GC: 0x16E4)
struct LGWheels {
    LGPosition Position[4]; // +0x0
    short NonLinearWheel[4][256]; // +0x28
    struct Wheels wheels; // +0x828
    struct Force force; // +0x10A8
    struct Condition condition; // +0x1148
    struct Constant constant; // +0x11E8
    struct Periodic periodic; // +0x1288
    struct Ramp ramp; // +0x1328
    unsigned char OverallGain; // +0x13C8
    bool damperWasPlaying[4]; // +0x13C9
    bool springWasPlaying[4]; // +0x13CD
    bool wasPlayingBeforeAirborne[4][10]; // +0x13D1
    bool IsAirborne[4]; // +0x13F9
    char _pad0[0x3]; // +0x13FD
    char SpringForceParams[16]; // +0x1400  anonymer Union/Struct, GC-Größe
    char ConstantForceParams[16]; // +0x1410  anonymer Union/Struct, GC-Größe
    char DamperForceParams[8]; // +0x1420  anonymer Union/Struct, GC-Größe
    char SideCollisionParams[16]; // +0x1428  anonymer Union/Struct, GC-Größe
    char FrontalCollisionParams[8]; // +0x1438  anonymer Union/Struct, GC-Größe
    char DirtRoadParams[8]; // +0x1440  anonymer Union/Struct, GC-Größe
    char BumpyRoadParams[8]; // +0x1448  anonymer Union/Struct, GC-Größe
    char SlipperyRoadParams[8]; // +0x1450  anonymer Union/Struct, GC-Größe
    char SurfaceEffectParams[16]; // +0x1458  anonymer Union/Struct, GC-Größe
};
// size 0x1
struct LString {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x10
struct LZHeader {
    unsigned int ID; // +0x0
    unsigned char Version; // +0x4
    unsigned char HeaderSize; // +0x5
    unsigned short Flags; // +0x6
    unsigned int UncompressedSize; // +0x8
    unsigned int CompressedSize; // +0xC
};
// size 0x1
struct LapSnapshotInfo {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x8
struct Linear {
    float m; // +0x0
    float b; // +0x4
};
// size 0x8
struct bTList_ListPicker__ListPickerItem_ {
    struct bList base; // +0x0  Basisklasse
};
// size 0x18
struct ListPicker {
    void **vptr; // +0x0  Vptr (MSVC: am Anfang)
    struct bTList_ListPicker__ListPickerItem_ ItemList; // +0x4
    struct ListPickerItem *CurrentItem; // +0xC
    struct FEObject *DisplayObject; // +0x10
    struct FEObject *ButtonObject; // +0x14
};
// size 0x8
struct bTNode_ListPicker__ListPickerItem_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0x14
struct ListPickerItem {
    void **vptr; // +0x0  Vptr (MSVC: am Anfang)
    struct bTNode_ListPicker__ListPickerItem_ base; // +0x4  Basisklasse
    int State; // +0xC
    const char *String; // +0x10
};
// size 0xC
struct LongVector {
    int x; // +0x0
    int y; // +0x4
    int z; // +0x8
};
// size 0x24
union MATRIX3 {
    float m[9]; // +0x0
    float m33[3][3]; // +0x0
};
// size 0x40
union MATRIX4 {
    float m[16]; // +0x0
    float m44[4][4]; // +0x0
};
// size 0x1
struct MEMCLASS {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x1C
struct MUTEX {
    int reserved[7]; // +0x0
};
// size 0x8
struct bTNode_MagazineBackground_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0x14
struct MagazineBackground {
    struct bTNode_MagazineBackground_ base; // +0x0  Basisklasse
    unsigned int BackgroundHash; // +0x8
    unsigned int TextureHash; // +0xC
    enum eBackgroundStyle BackgroundStyle; // +0x10
};
// size 0x8
struct bTNode_MagazineCamera_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0x38
struct MagazineCamera {
    struct bTNode_MagazineCamera_ base; // +0x0  Basisklasse
    unsigned int CameraHash; // +0x8
    float Orientation_H; // +0xC
    float Orientation_V; // +0x10
    float Orientation_Z; // +0x14
    float Offset_X; // +0x18
    float Offset_Y; // +0x1C
    float Offset_Z; // +0x20
    float FOV; // +0x24
    float RollAngle; // +0x28
    float LookAt_X; // +0x2C
    float LookAt_Y; // +0x30
    float LookAt_Z; // +0x34
};
// size 0x8
struct bTNode_MagazineEvent_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0xC
struct MagazineEvent {
    struct bTNode_MagazineEvent_ base; // +0x0  Basisklasse
    unsigned int EventHash; // +0x8
};
// size 0x8
struct bTNode_MagazineFemale_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0x10
struct MagazineFemale {
    struct bTNode_MagazineFemale_ base; // +0x0  Basisklasse
    unsigned int GirlHash; // +0x8
    unsigned int TextureHash; // +0xC
};
// size 0x8
struct bTNode_MagazineHeadline_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0xA8
struct MagazineHeadline {
    struct bTNode_MagazineHeadline_ base; // +0x0  Basisklasse
    char Headline[64]; // +0x8
    char Headline_After_Car_Name[64]; // +0x48
    unsigned int Hash; // +0x88
    unsigned int IsCarSpecific; // +0x8C
    unsigned int IsTwoParts; // +0x90
    float PosX; // +0x94
    float PosY; // +0x98
    float ScaleX; // +0x9C
    float ScaleY; // +0xA0
    unsigned char ColorR; // +0xA4
    unsigned char ColorG; // +0xA5
    unsigned char ColorB; // +0xA6
    char _pad0[0x1]; // +0xA7
};
// size 0x8
struct bTNode_MagazineName_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0x10
struct MagazineName {
    struct bTNode_MagazineName_ base; // +0x0  Basisklasse
    unsigned int NameHash; // +0x8
    unsigned int TextureHash; // +0xC
};
// size 0x8
struct MagazineName2ThumbMapping {
    unsigned int MagazineNameHash; // +0x0
    unsigned int ThumbImageNameHash; // +0x4
};
// size 0x40
struct MagazineScreenDebugData {
    int ComboIx; // +0x0
    int NumCombinationsTotal; // +0x4
    int NumCombinationsOtherSetups_Before; // +0x8
    int NumSetups; // +0xC
    int NumMastheads; // +0x10
    int NumHeadlines; // +0x14
    int NumBackgrounds; // +0x18
    int NumCameras; // +0x1C
    int NumFemales; // +0x20
    int EventIndex; // +0x24
    int MagazineSetupIndex; // +0x28
    int MastheadIndex; // +0x2C
    int HeadlineIndex; // +0x30
    int BackgroundIndex; // +0x34
    int CameraIndex; // +0x38
    int FemaleIndex; // +0x3C
};
// size 0x8
struct bTNode_MagazineSetup_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0x168
struct MagazineSetup {
    struct bTNode_MagazineSetup_ base; // +0x0  Basisklasse
    unsigned int MagSetupHash; // +0x8
    unsigned int MagSetupFlags; // +0xC
    int NumMastheads; // +0x10
    int NumHeadlines; // +0x14
    int NumGirls; // +0x18
    int NumBackgrounds; // +0x1C
    int NumCameraAngles; // +0x20
    unsigned int ChooseFromAllFlags; // +0x24
    int PossibleMastheads[16]; // +0x28
    int PossibleHeadlines[16]; // +0x68
    int PossibleGirls[16]; // +0xA8
    int PossibleBackgrounds[16]; // +0xE8
    int PossibleCameraAngles[16]; // +0x128
};
// size 0x8
struct bTList_MagazineEvent_ {
    struct bList base; // +0x0  Basisklasse
};
// size 0x8
struct bTList_MagazineName_ {
    struct bList base; // +0x0  Basisklasse
};
// size 0x8
struct bTList_MagazineHeadline_ {
    struct bList base; // +0x0  Basisklasse
};
// size 0x8
struct bTList_MagazineFemale_ {
    struct bList base; // +0x0  Basisklasse
};
// size 0x8
struct bTList_MagazineBackground_ {
    struct bList base; // +0x0  Basisklasse
};
// size 0x8
struct bTList_MagazineCamera_ {
    struct bList base; // +0x0  Basisklasse
};
// size 0x8
struct bTList_MagazineSetup_ {
    struct bList base; // +0x0  Basisklasse
};
// size 0x8
struct bTList_EventSetup_ {
    struct bList base; // +0x0  Basisklasse
};
// size 0x44
struct MagazineTemplateDatabase {
    void **vptr; // +0x0  Vptr (MSVC: am Anfang)
    struct bTList_MagazineEvent_ EventList; // +0x4
    struct bTList_MagazineName_ NameList; // +0xC
    struct bTList_MagazineHeadline_ Headlines; // +0x14
    struct bTList_MagazineFemale_ Females; // +0x1C
    struct bTList_MagazineBackground_ Backgrounds; // +0x24
    struct bTList_MagazineCamera_ Cameras; // +0x2C
    struct bTList_MagazineSetup_ MagazineSetups; // +0x34
    struct bTList_EventSetup_ EventSetups; // +0x3C
};
// size 0xC
struct MatchPhaseInput {
    float mAngle; // +0x0
    float mDAngle; // +0x4
    float mSearchLength; // +0x8
};
// size 0x18
struct MemBlockInfo_t {
    const char *name; // +0x0
    int size; // +0x4
    int alignment; // +0x8
    int alignmentOffset; // +0xC
    int flags; // +0x10
    void *addr; // +0x14
};
// size 0x8
struct bTList_FreeBlock_ {
    struct bList base; // +0x0  Basisklasse
};
// size 0x8
struct bTList_AllocationHeader_ {
    struct bList base; // +0x0  Basisklasse
};
// size 0x50 (GC: 0x54)
struct MemoryPool {
    const char *pDebugName; // +0x0
    struct bTList_FreeBlock_ FreeBlockList; // +0x4
    struct bTList_AllocationHeader_ AllocationHeaderList; // +0xC
    int NumAllocations; // +0x14
    int TotalNumAllocations; // +0x18
    int PoolSize; // +0x1C
    int AmountAllocated; // +0x20
    int MostAmountAllocated; // +0x24
    int AmountFree; // +0x28
    int LeastAmountFree; // +0x2C
    bool DebugFillEnabled; // +0x30
    bool DebugTracingEnabled; // +0x31
    char _pad0[0x2]; // +0x32
    struct MUTEX Mutex; // +0x34
};
// size 0x4
struct MemoryPoolManager {
    void **vptr; // +0x0  Vptr (MSVC: am Anfang)
};
// size 0xC
struct SoundImplementer {
    void **vptr; // +0x0  Vptr (MSVC: am Anfang)
    bool PlayedSpeech; // +0x4
    char _pad0[0x3]; // +0x5
    unsigned int PackageHash; // +0x8
};
// size 0x10
struct ScreenConstructorData {
    const char *PackageName; // +0x0
    struct FEPackage *pPackage; // +0x4
    struct PersistentMenuScreenData *PersistentData; // +0x8
    unsigned int HelpHash; // +0xC
};
// size 0x3C
struct MenuScreen {
    struct SoundImplementer base; // +0x0  Basisklasse
    const char *PackageName; // +0xC
    struct ScreenConstructorData ConstructData; // +0x10
    bool IsGarageScreen; // +0x20
    char _pad0[0x3]; // +0x21
    struct FEObject *RightButton; // +0x24
    struct FEObject *LeftButton; // +0x28
    struct FEObject *UpButton; // +0x2C
    struct FEObject *DownButton; // +0x30
    bool MenuBarIsActive; // +0x34
    char _pad1[0x3]; // +0x35
    unsigned int uiHelpHash; // +0x38
};
// size 0x6C
struct MiscCarTunings {
    float DriftYawMomentCoeff[3]; // +0x0
    float DriftYawAngularVelCoeff; // +0xC
    float DriftTractionSplit; // +0x10
    float DriftTractionPercent[4]; // +0x14
    float DriftWeightCoeff[4]; // +0x24
    float DriftInertiaZTensorCoeff[4]; // +0x34
    float DriftAeroReduction; // +0x44
    float DriftTorqueSplit; // +0x48
    float BurnoutFrictionCoeff[4]; // +0x4C
    float PerfectShiftNeutralMultiplier; // +0x5C
    float RoadTorqueBalanceFactorNormal; // +0x60
    float RoadTorqueBalanceFactorDrift; // +0x64
    float RoadTorqueBalanceFactorNeutral; // +0x68
};
// size 0x1
struct MobileTopology {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x4 (GC: 0xC)
struct MtxStack {
    unsigned int numMtx; // +0x0
};
// size 0x28
struct OSAlarm {
    void (* handler)(struct OSAlarm *, OSContext *); // +0x0
    unsigned int tag; // +0x4
    long long fire; // +0x8
    struct OSAlarm *prev; // +0x10
    struct OSAlarm *next; // +0x14
    long long period; // +0x18
    long long start; // +0x20
};
// size 0x28
struct OSCalendarTime {
    int sec; // +0x0
    int min; // +0x4
    int hour; // +0x8
    int mday; // +0xC
    int mon; // +0x10
    int year; // +0x14
    int wday; // +0x18
    int yday; // +0x1C
    int msec; // +0x20
    int usec; // +0x24
};
// size 0x8
struct OSThreadQueue {
    OSThread *head; // +0x0
    OSThread *tail; // +0x4
};
// size 0x8
struct OSCond {
    OSThreadQueue queue; // +0x0
};
// size 0x2C8
struct OSContext {
    unsigned long gpr[32]; // +0x0
    unsigned int cr; // +0x80
    unsigned int lr; // +0x84
    unsigned int ctr; // +0x88
    unsigned int xer; // +0x8C
    double fpr[32]; // +0x90
    unsigned int fpscr_pad; // +0x190
    unsigned int fpscr; // +0x194
    unsigned int srr0; // +0x198
    unsigned int srr1; // +0x19C
    unsigned short mode; // +0x1A0
    unsigned short state; // +0x1A2
    unsigned long gqr[8]; // +0x1A4
    unsigned int psf_pad; // +0x1C4
    double psf[32]; // +0x1C8
};
// size 0x30
struct OSFontHeader {
    unsigned short fontType; // +0x0
    unsigned short firstChar; // +0x2
    unsigned short lastChar; // +0x4
    unsigned short invalChar; // +0x6
    unsigned short ascent; // +0x8
    unsigned short descent; // +0xA
    unsigned short width; // +0xC
    unsigned short leading; // +0xE
    unsigned short cellWidth; // +0x10
    unsigned short cellHeight; // +0x12
    unsigned int sheetSize; // +0x14
    unsigned short sheetFormat; // +0x18
    unsigned short sheetColumn; // +0x1A
    unsigned short sheetRow; // +0x1C
    unsigned short sheetWidth; // +0x1E
    unsigned short sheetHeight; // +0x20
    unsigned short widthTable; // +0x22
    unsigned int sheetImage; // +0x24
    unsigned int sheetFullSize; // +0x28
    unsigned char c0; // +0x2C
    unsigned char c1; // +0x2D
    unsigned char c2; // +0x2E
    unsigned char c3; // +0x2F
};
// size 0x8
struct OSImportInfo {
    unsigned int id; // +0x0
    unsigned int offset; // +0x4
};
// size 0x20
struct OSMessageQueue {
    OSThreadQueue queueSend; // +0x0
    OSThreadQueue queueReceive; // +0x8
    void **msgArray; // +0x10
    int msgCount; // +0x14
    int firstIndex; // +0x18
    int usedCount; // +0x1C
};
// size 0x8
struct OSModuleLink {
    struct OSModuleInfo *next; // +0x0
    struct OSModuleInfo *prev; // +0x4
};
// size 0x20
struct OSModuleInfo {
    unsigned int id; // +0x0
    struct OSModuleLink link; // +0x4
    unsigned int numSections; // +0xC
    unsigned int sectionInfoOffset; // +0x10
    unsigned int nameOffset; // +0x14
    unsigned int nameSize; // +0x18
    unsigned int version; // +0x1C
};
// size 0x4C
struct OSModuleHeader {
    struct OSModuleInfo info; // +0x0
    unsigned int bssSize; // +0x20
    unsigned int relOffset; // +0x24
    unsigned int impOffset; // +0x28
    unsigned int impSize; // +0x2C
    unsigned char prologSection; // +0x30
    unsigned char epilogSection; // +0x31
    unsigned char unresolvedSection; // +0x32
    unsigned char bssSection; // +0x33
    unsigned int prolog; // +0x34
    unsigned int epilog; // +0x38
    unsigned int unresolved; // +0x3C
    unsigned int align; // +0x40
    unsigned int bssAlign; // +0x44
    unsigned int fixSize; // +0x48
};
// size 0x8
struct OSModuleQueue {
    struct OSModuleInfo *head; // +0x0
    struct OSModuleInfo *tail; // +0x4
};
// size 0x8
struct OSMutexLink {
    struct OSMutex *next; // +0x0
    struct OSMutex *prev; // +0x4
};
// size 0x18
struct OSMutex {
    struct OSThreadQueue queue; // +0x0
    OSThread *thread; // +0x8
    int count; // +0xC
    struct OSMutexLink link; // +0x10
};
// size 0x8
struct OSMutexQueue {
    struct OSMutex *head; // +0x0
    struct OSMutex *tail; // +0x4
};
// size 0x8
struct OSRel {
    unsigned short offset; // +0x0
    unsigned char type; // +0x2
    unsigned char section; // +0x3
    unsigned int addend; // +0x4
};
// size 0x10
struct OSResetFunctionInfo {
    int (* func)(int); // +0x0
    unsigned int priority; // +0x4
    struct OSResetFunctionInfo *next; // +0x8
    struct OSResetFunctionInfo *prev; // +0xC
};
// size 0x8
struct OSSectionInfo {
    unsigned int offset; // +0x0
    unsigned int size; // +0x4
};
// size 0xC
struct OSSemaphore {
    int count; // +0x0
    OSThreadQueue queue; // +0x4
};
// size 0x30
struct OSStopwatch {
    char *name; // +0x0
    unsigned int hits; // +0x4
    long long total; // +0x8
    long long min; // +0x10
    long long max; // +0x18
    long long last; // +0x20
    int running; // +0x28
    unsigned int _padding; // +0x2C
};
// size 0x8
struct OSThreadLink {
    OSThread *next; // +0x0
    OSThread *prev; // +0x4
};
// size 0x318
struct OSThread {
    OSContext context; // +0x0
    unsigned short state; // +0x2C8
    unsigned short attr; // +0x2CA
    int suspend; // +0x2CC
    int priority; // +0x2D0
    int base; // +0x2D4
    void *val; // +0x2D8
    struct OSThreadQueue *queue; // +0x2DC
    struct OSThreadLink link; // +0x2E0
    struct OSThreadQueue queueJoin; // +0x2E8
    struct OSMutex *mutex; // +0x2F0
    struct OSMutexQueue queueMutex; // +0x2F4
    struct OSThreadLink linkActive; // +0x2FC
    unsigned char *stackBase; // +0x304
    unsigned long *stackEnd; // +0x308
    int error; // +0x30C
    void *specific[2]; // +0x310
};
// size 0x1
struct ObjectPool_FEKeyNode_256_ {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x1
struct ObjectPool_FEMessageResponse_64_ {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x1
struct ObjectPool_FEScript_32_ {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x4
struct PPC_DMA_L_t {
    unsigned int _bits_0x0; // +0x0  Bitfelder lcAddr:27 dmaLd:1 dmaLenL:2 dmaTrigger:1 dmaFlush:1
};
// size 0x4
union PPC_DMA_L_u {
    unsigned int val; // +0x0
    PPC_DMA_L_t f; // +0x0
};
// size 0x4
struct PPC_DMA_U_t {
    unsigned int _bits_0x0; // +0x0  Bitfelder memAddr:27 dmaLenU:5
};
// size 0x4
union PPC_DMA_U_u {
    unsigned int val; // +0x0
    PPC_DMA_U_t f; // +0x0
};
// size 0x4
struct PPC_GQR_t {
    unsigned int _bits_0x0; // +0x0  Bitfelder _pad0:2 loadScale:6 _pad1:5 loadType:3 _pad2:2 storeScale:6 _pad3:5 storeType:3
};
// size 0x4
union PPC_GQR_u {
    unsigned int val; // +0x0
    PPC_GQR_t f; // +0x0
};
// size 0x4
union Parameter {
    int iVal; // +0x0
    float fVal; // +0x0
};
// size 0x1
struct PerfMeter {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x4
struct PersistentMenuScreenData {
    int CurrentActiveObject; // +0x0
};
// size 0x1
struct PhaseChan {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x4
struct PhaseValue {
    float mAngle; // +0x0
};
// size 0x1
struct PhysicsSystem {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x58
struct PhysicsUpgradeSpecification {
    int UpgradeLevel[11]; // +0x0
    int JunkmanPart[11]; // +0x2C
};
// size 0x84
struct PidError {
    struct Average aTimes; // +0x0
    struct Average aIntegral; // +0x28
    struct Average aDerivative; // +0x50
    float fFrequency; // +0x78
    float fCurrentError; // +0x7C
    float fPreviousError; // +0x80
};
// size 0x1
struct PostRaceCameraCoordinator {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x1
struct PreRaceCameraCoordinator {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x1
struct PreviewRoute {
    char _empty; // +0x0 (leere Klasse)
};
// size 0xC
struct ProfileDataEntry {
    const char *SectionName; // +0x0
    int Depth; // +0x4
    unsigned int Ticks; // +0x8
};
// size 0x1
struct ProfileManager {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x1
struct ProfileNode {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x1
struct ProfileSectionEntry {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x10
struct Quaternion {
    float x; // +0x0
    float y; // +0x4
    float z; // +0x8
    float w; // +0xC
};
// size 0x8
struct bTNode_QueuedFile_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0x74
struct QueuedFile {
    struct bTNode_QueuedFile_ base; // +0x0  Basisklasse
    void *pBuf; // +0x8
    char Filename[48]; // +0xC
    int FilePos; // +0x3C
    int NumBytes; // +0x40
    int NumRead; // +0x44
    int BlockSize; // +0x48
    int Priority; // +0x4C
    int DisableJoylog; // +0x50
    void *CallbackFunction; // +0x54
    int CallbackParam; // +0x58
    int CallbackParam2; // +0x5C
    int CallbackModeUseParam2; // +0x60
    int Handle; // +0x64
    enum QueuedFileStatus Status; // +0x68
    int AbortFlag; // +0x6C
    float StartReadTime; // +0x70
};
// size 0x54
struct QueuedFileBundle {
    signed char *ReadBuffer; // +0x0
    int ReadBufferBot; // +0x4
    int ReadBufferTop; // +0x8
    int NumBytesQueued; // +0xC
    int NumQueuedFiles; // +0x10
    struct QueuedFile *QueuedFiles[16]; // +0x14
};
// size 0x2C
struct QuickSpline {
    unsigned int nHash; // +0x0
    enum QuickSplineEndPointType EndPointType; // +0x4
    enum QuickSplineBasisType BasisType; // +0x8
    float MaxParam; // +0xC
    float MinParam; // +0x10
    float Length; // +0x14
    char ControlPointsDirty; // +0x18
    char BufferWasAllocated; // +0x19
    char MinControlPoints; // +0x1A
    char _pad0[0x1]; // +0x1B
    unsigned short MaxControlPoints; // +0x1C
    unsigned short NumControlPoints; // +0x1E
    struct bVector4 *pControlPointBuffer; // +0x20
    struct bVector4 *pControlPoints; // +0x24
    struct bMatrix4 *pBasisMatricies; // +0x28
};
// size 0x8
struct bTNode_RCQueuedMessage_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0x10
struct RCQueuedMessage {
    struct bTNode_RCQueuedMessage_ base; // +0x0  Basisklasse
    enum RCMessage Msg; // +0x8
    int Param; // +0xC
};
// size 0x4
struct REALELEMENT {
    struct REALELEMENT *link; // +0x0
};
// size 0x34
struct REALQUEUE {
    unsigned int count; // +0x0
    unsigned int flags; // +0x4
    REALELEMENT *head; // +0x8
    REALELEMENT *tail; // +0xC
    QUEUEFUNC *prifunc; // +0x10
    void *context; // +0x14
    MUTEX mutex; // +0x18
};
// size 0x8
struct REQUESTQUEUE {
    struct REQUESTSTRUCTtag *head; // +0x0
    struct REQUESTSTRUCTtag *tail; // +0x4
};
// size 0x30
struct REQUESTSTRUCTtag {
    int id; // +0x0
    struct REQUESTSTRUCTtag *nextreq; // +0x4
    int bytesread; // +0x8
    int releaseflag; // +0xC
    int cancelflag; // +0x10
    char *loadaddr; // +0x14
    void (* callback)(int); // +0x18
    volatile int fop; // +0x1C
    int fhandle; // +0x20
    int foffset; // +0x24
    int readsize; // +0x28
    char *destaddr; // +0x2C
};
// size 0x8
struct bTList_RCQueuedMessage_ {
    struct bList base; // +0x0  Basisklasse
};
// size 0xB4 (GC: 0xB8)
struct RaceCoordinator {
    enum RCState CurrentRCState; // +0x0
    struct Timer CurrentRCStateStartTime; // +0x4
    struct bTList_RCQueuedMessage_ RCQueuedMessageList; // +0x8
    int RCExecutingMessage; // +0x10
    enum RCState InitialRCState; // +0x14
    int InitialRaceStage; // +0x18
    int DisplayingCountdownNumber; // +0x1C
    int PausedJoystickPort; // +0x20
    enum RCState BeforePauseRCState; // +0x24
    bool bAlreadyPaused; // +0x28
    bool bPauseRequestedDuringNIS; // +0x29
    char _pad0[0x2]; // +0x2A
    enum RCState BeforeTutorialState; // +0x2C
    unsigned int CurrentTutorialMessage; // +0x30
    struct DemoCameraCoordinator *pDemoCameraCoordinator; // +0x34
    struct FlyByCameraCoordinator *pFlyByCameraCoordinator; // +0x38
    struct PreRaceCameraCoordinator *pPreRaceCameraCoordinator; // +0x3C
    struct IntroNisCameraCoordinator *pIntroNisCameraCoordinator; // +0x40
    struct EndingNisCameraCoordinator *pEndingNisCameraCoordinator; // +0x44
    int BeforeFirstRace; // +0x48
    struct cPostRaceScreenMgrInterface *pPostRaceScreenMgr; // +0x4C
    struct ReplayScreen *pReplayScreen; // +0x50
    int mNumRewinds; // +0x54
    struct CameraMover *mpCameraMover; // +0x58
    struct CameraDataItems *mpCD; // +0x5C
    struct TrackRoutePoint *mIncidentPoint; // +0x60
    struct bVector3 mIncidentCoord; // +0x64
    struct bVector3 mFinalPosition; // +0x74
    struct bVector3 mStartPosition; // +0x84
    struct bVector3 *mpEyeVerts; // +0x94
    struct bVector3 *mpBankVerts; // +0x98
    struct bVector3 *mpFOVtable; // +0x9C
    struct Car *mpHighlightCar; // +0xA0
    struct Car *mpHighlightCar2; // +0xA4
    int mViewID; // +0xA8
    struct PreviewRoute *mpRoute; // +0xAC
    int mDemoPlayerNumber; // +0xB0
};
// size 0x804C (GC: 0x8060)
struct ReadStatus {
    FileReadStateEnum CurState; // +0x0
    int MemBase; // +0x4
    int FileBase; // +0x8
    int Size; // +0xC
    int MBS; // +0x10
    int MSA; // +0x14
    int ME; // +0x18
    int MEA; // +0x1C
    int FBS; // +0x20
    int FSA; // +0x24
    int FE; // +0x28
    int FEA; // +0x2C
    int SD; // +0x30
    int TA; // +0x34
    int ret; // +0x38
    volatile HANDLE *handle; // +0x3C
    volatile void *MemPointer; // +0x40
    volatile long parmFileBase; // +0x44
    volatile long parmSize; // +0x48
    char Data[32768]; // +0x4C
};
// size 0x8
struct bTNode_RecyclingThumbnailNode_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0x20
struct RecyclingThumbnailNode {
    struct bTNode_RecyclingThumbnailNode_ base; // +0x0  Basisklasse
    unsigned int Item; // +0x8
    unsigned int TextureHash; // +0xC
    struct FEObject *FEngObject; // +0x10
    float XPos; // +0x14
    unsigned int OriginalColor; // +0x18
    bool IsGreyOut; // +0x1C
    char _pad0[0x3]; // +0x1D
};
// size 0x8
struct bTList_ThumbObject_ {
    struct bList base; // +0x0  Basisklasse
};
// size 0x8
struct bTList_RecyclingThumbnailNode_ {
    struct bList base; // +0x0  Basisklasse
};
// size 0x54 (GC: 0x58)
struct RecyclingThumbnailScroller {
    void **vptr; // +0x0  Vptr (MSVC: am Anfang)
    bool DoneAddingNodes; // +0x4
    char _pad0[0x3]; // +0x5
    struct bTList_ThumbObject_ ThumbObjectList; // +0x8
    struct bTList_ThumbObject_ CyclingObjectList; // +0x10
    struct bTList_RecyclingThumbnailNode_ NodeList; // +0x18
    struct RecyclingThumbnailNode *CurrentNode; // +0x20
    struct ThumbObject *CurrentThumbObject; // +0x24
    bool SplineLeft; // +0x28
    bool SplineStarted; // +0x29
    char _pad1[0x2]; // +0x2A
    struct tCubic1D *AnimateCubic; // +0x2C
    float SelectedX; // +0x30
    float SelectedY; // +0x34
    float CurrentAddPos; // +0x38
    int NumThumbObjectsInPackage; // +0x3C
    const char *PackageName; // +0x40
    const char *GroupName; // +0x44
    struct FEObject *MasterObject; // +0x48
    struct FEObject *CurrentAlloc; // +0x4C
    float PulseState; // +0x50
};
// size 0x1
struct RenderContext {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x1
struct RenderContextBase {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x1
struct RenderContextExtension {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x1
struct RenderContextPrivate {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x1
struct RenderMethod {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x8
struct bTList_ReplaySnapshot_ {
    struct bList base; // +0x0  Basisklasse
};
// size 0x10
struct ReplaySpecialEventInfo {
    enum eReplaySpecialEvent Event; // +0x0
    int WorldLoopCounter; // +0x4
    int Param1; // +0x8
    int Param2; // +0xC
};
// size 0x50B8
struct ReplayManager {
    int ReplayingFlag; // +0x0
    int CapturingFlag; // +0x4
    int RemoveDataError; // +0x8
    int ChecksumErrorFlag; // +0xC
    int PausedFlag; // +0x10
    int SlowMotionFlag; // +0x14
    int ReplayPausedThisFrame; // +0x18
    int NextFancyDebugTime; // +0x1C
    int FancyReplayInProgress; // +0x20
    int FancyJoyEventNumber; // +0x24
    int NumFancyJoyEvents; // +0x28
    unsigned char FancyJoyEventTable[32]; // +0x2C
    int NextInstantReplaySnapshotTime; // +0x4C
    int BeginCapturingLoopCount; // +0x50
    int EndCapturingLoopCount; // +0x54
    int FinishedAllCapturing; // +0x58
    struct Timer BeginningOfTime; // +0x5C
    struct Timer EndOfTime; // +0x60
    struct bRepeat *pRepeat; // +0x64
    struct bTList_ReplaySnapshot_ FreeReplaySnapshotList; // +0x68
    struct bTList_ReplaySnapshot_ ReplaySnapshotList; // +0x70
    int MaxNumSnapshots; // +0x78
    int MaxNumInstantReplaySnapshots; // +0x7C
    int MaxNumAutoReplaySnapshots; // +0x80
    int MaxNumLapSnapshots; // +0x84
    int MaxNumHighlightSnapshots; // +0x88
    int NumInstantReplaySnapshots; // +0x8C
    int NumLapSnapshots; // +0x90
    int NumHighlightSnapshots; // +0x94
    float LapSnapshotSpacing; // +0x98
    int NextHighlightNumber; // +0x9C
    int EndCurrentHighlightTime; // +0xA0
    int LastSnapshotTime; // +0xA4
    int NumSnapshotsTaken; // +0xA8
    int LastSnapshotIndex; // +0xAC
    int SnapshotTimeTable[4096]; // +0xB0
    int NumSpecialEvents; // +0x40B0
    int LastSpecialEventNumber; // +0x40B4
    struct ReplaySpecialEventInfo SpecialEventTable[256]; // +0x40B8
};
// size 0x1
struct ReplayScreen {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x8
struct bTNode_ReplaySnapshot_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0x10
struct ReplaySnapshotSectionInfo {
    const char *Name; // +0x0
    void *Object; // +0x4
    short Number; // +0x8
    char NumMoveData; // +0xA
    char ChecksumErrorDetected; // +0xB
    int Size; // +0xC
};
// size 0x9558
struct ReplaySnapshot {
    struct bTNode_ReplaySnapshot_ base; // +0x0  Basisklasse
    char Name[32]; // +0x8
    struct Timer BeginTime; // +0x28
    struct Timer EndTime; // +0x2C
    int BeginLoopTime; // +0x30
    int EndLoopTime; // +0x34
    int MemoryImageLoadedFlag; // +0x38
    char TakingSnapshotFlag; // +0x3C
    char RestoringSnapshotFlag; // +0x3D
    char TakingChecksumFlag; // +0x3E
    char VerifyingChecksumFlag; // +0x3F
    char CurrentSectionDepth; // +0x40
    char OverflowedFlag; // +0x41
    short CurrentSectionNumber; // +0x42
    int PrintDepth; // +0x44
    int PrintLoopCounter; // +0x48
    struct ReplaySnapshotSectionInfo SectionInfo[16]; // +0x4C
    float TotalInterestingValue; // +0x14C
    int DataPosition; // +0x150
    int SizeofData; // +0x154
    unsigned char Data[37888]; // +0x158
};
// size 0x1
struct RigidBody {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x14
struct RigidBodyMover {
    struct Mover base; // +0x0  Basisklasse
};
// size 0x28
struct RuntimeAllocConstructorPool {
    struct SymbolPool conspool; // +0x0
    struct SymbolPool despool; // +0x14
};
// size 0x1
struct RuntimeAllocDestructorEntry {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x6
struct S16Vec {
    short x; // +0x0
    short y; // +0x2
    short z; // +0x4
};
// size 0x14
struct SCATTERREQUEST {
    int num; // +0x0
    const char **names; // +0x4
    void **buffers; // +0x8
    int flags; // +0xC
    int abortflag; // +0x10
};
// size 0x8
struct SCOORD {
    int x; // +0x0
    int y; // +0x4
};
// size 0x10
struct SHAPE {
    unsigned int _bits_0x0; // +0x0  Bitfelder type:8 next:24
    short width; // +0x4
    short height; // +0x6
    short centerx; // +0x8
    short centery; // +0xA
    unsigned int _bits_0xc; // +0xC  Bitfelder reserved:1 transposed:1 swizzled:1 referenced:1 shapex:12 mipmaps:4 shapey:12
};
// size 0x8
struct SHAPETABLEENTRY {
    char id[4]; // +0x0
    int offset; // +0x4
};
// size 0x18
struct SHAPEFILE {
    char type[4]; // +0x0
    int len; // +0x4
    int num; // +0x8
    char creator[4]; // +0xC
    SHAPETABLEENTRY tbl[1]; // +0x10
};
// size 0x28
struct SIGNAL {
    int reserved[10]; // +0x0
};
// size 0x8
union SN_PS_type {
    double ps; // +0x0
};
// size 0x8
struct STREAMCHUNKHDR {
    int type; // +0x0
    int size; // +0x4
};
// size 0x50
struct STRUCT_DSP_TASK {
    volatile unsigned long state; // +0x0
    volatile unsigned long priority; // +0x4
    volatile unsigned long flags; // +0x8
    unsigned short *iram_mmem_addr; // +0xC
    unsigned int iram_length; // +0x10
    unsigned int iram_addr; // +0x14
    unsigned short *dram_mmem_addr; // +0x18
    unsigned int dram_length; // +0x1C
    unsigned int dram_addr; // +0x20
    unsigned short dsp_init_vector; // +0x24
    unsigned short dsp_resume_vector; // +0x26
    void (* init_cb)(void *); // +0x28
    void (* res_cb)(void *); // +0x2C
    void (* done_cb)(void *); // +0x30
    void (* req_cb)(void *); // +0x34
    struct STRUCT_DSP_TASK *next; // +0x38
    struct STRUCT_DSP_TASK *prev; // +0x3C
    long long t_context; // +0x40
    long long t_task; // +0x48
};
// size 0x24
struct SYNC_TRANSFER_OP {
    volatile int priority; // +0x0
    volatile int handle; // +0x4
    volatile int offset; // +0x8
    volatile int amount; // +0xC
    volatile int result; // +0x10
    volatile int requested; // +0x14
    volatile void *buffer; // +0x18
    int (* opfunc)(int, int, void *, int, int, void *); // +0x1C
    int fileop; // +0x20
};
// size 0x14
struct SampleMover {
    struct Mover base; // +0x0  Basisklasse
};
// size 0x20
struct ScannerConfig {
    char ConfigurationNumbers[8]; // +0x0
    enum JoyEvent Event; // +0x8
    unsigned short (* Scanner)(struct EventNode *, unsigned int *, unsigned int *, struct ScannerConfig *, struct Joystick *); // +0xC
    unsigned char Xor1; // +0x10
    unsigned char Xor2; // +0x11
    unsigned char Shift1; // +0x12
    unsigned char Shift2; // +0x13
    unsigned char Param; // +0x14
    unsigned char AnalogDeviceNumber; // +0x15
    unsigned char ValidDuringReplay; // +0x16
    char _pad0[0x1]; // +0x17
    unsigned int ButtonTexture; // +0x18
    char *ButtonName; // +0x1C
};
// size 0xC
struct SceneryBoundingBox {
    short BBoxMin[3]; // +0x0
    short BBoxMax[3]; // +0x6
};
// size 0xC
struct SceneryDrawInfo {
    struct eModel *pModel; // +0x0
    struct bMatrix4 *pMatrix; // +0x4
    struct SceneryInstance *SceneryInst; // +0x8
};
// size 0x48
struct SceneryInfo {
    unsigned int NameHash[6]; // +0x0
    short FarClipSize[4]; // +0x18
    short Dummy; // +0x20
    char _pad0[0x2]; // +0x22
    struct eModel *pModel[6]; // +0x24
    char IsFacadeFlag[6]; // +0x3C
    char Dummy2; // +0x42
    char _pad1[0x1]; // +0x43
    float Radius; // +0x44
};
// size 0x30
struct SceneryInstance {
    struct SceneryBoundingBox base; // +0x0  Basisklasse
    short SceneryInfoNumber; // +0xC
    short ExcludeFlags; // +0xE
    float Position[3]; // +0x10
    short Rotation[9]; // +0x1C
    short Pad; // +0x2E
};
// size 0x168
struct SceneryLogInfo {
    char MeshName[32]; // +0x0
    char InstanceMeshName[32]; // +0x20
    char BBGFilename[48]; // +0x40
    char SmokeableTypeName[32]; // +0x70
    unsigned int NameHash[6]; // +0x90
    struct bVector3 Position; // +0xA8
    struct bMatrix4 Matrix; // +0xB8
    struct bVector3 BBoxMin; // +0xF8
    struct bVector3 BBoxMax; // +0x108
    int SectionNumber; // +0x118
    int SceneryInfoNumber; // +0x11C
    int SceneryInstanceNumber; // +0x120
    int ExcludeFlags; // +0x124
    float Radius; // +0x128
    int SmokeableFlag; // +0x12C
    unsigned int SmokeableHash; // +0x130
    int FlattenIfDrivenOver; // +0x134
    int IncludedAsTopology; // +0x138
    int IsTrafficLight; // +0x13C
    char SoundEventType[32]; // +0x140
    int NumPolysWorld; // +0x160
    int NumPolysMemory; // +0x164
};
// size 0x8
struct bTNode_ScenerySectionHeader_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0x3C
struct ScenerySectionHeader {
    struct bTNode_ScenerySectionHeader_ base; // +0x0  Basisklasse
    int EndianSwapped; // +0x8
    int SectionNumber; // +0xC
    int NumPolygonsInMemory; // +0x10
    int NumPolygonsInWorld; // +0x14
    struct SceneryInfo *pSceneryInfo; // +0x18
    int NumSceneryInfo; // +0x1C
    struct SceneryInstance *pSceneryInstance; // +0x20
    int NumSceneryInstances; // +0x24
    struct SceneryTreeNode *SceneryTreeNodeTable; // +0x28
    int NumSceneryTreeNodes; // +0x2C
    struct SceneryTreeGroupNode *SceneryTreeGroupNodeTable; // +0x30
    int NumSceneryTreeGroupNodes; // +0x34
    int ViewsVisibleThisFrame; // +0x38
};
// size 0x1
struct SceneryTreeGroupNode {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x18
struct SceneryTreeNode {
    struct SceneryBoundingBox base; // +0x0  Basisklasse
    short NumChildren; // +0xC
    short ChildCodes[5]; // +0xE
};
// size 0x8
struct bTNode_ScrollerDatum_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0x8
struct bTList_ScrollerDatumNode_ {
    struct bList base; // +0x0  Basisklasse
};
// size 0x10
struct ScrollerDatum {
    struct bTNode_ScrollerDatum_ base; // +0x0  Basisklasse
    struct bTList_ScrollerDatumNode_ Strings; // +0x8
};
// size 0x8
struct bTNode_ScrollerDatumNode_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0x88
struct ScrollerDatumNode {
    struct bTNode_ScrollerDatumNode_ base; // +0x0  Basisklasse
    char String[128]; // +0x8
};
// size 0x8
struct bTNode_ScrollerSlot_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0x8
struct bTList_ScrollerSlotNode_ {
    struct bList base; // +0x0  Basisklasse
};
// size 0x14
struct ScrollerSlot {
    struct bTNode_ScrollerSlot_ base; // +0x0  Basisklasse
    struct bTList_ScrollerSlotNode_ FEStrings; // +0x8
    struct FEObject *pBacking; // +0x10
};
// size 0x8
struct bTNode_ScrollerSlotNode_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0xC
struct ScrollerSlotNode {
    struct bTNode_ScrollerSlotNode_ base; // +0x0  Basisklasse
    struct FEObject *String; // +0x8
};
// size 0x8
struct bTList_ScrollerSlot_ {
    struct bList base; // +0x0  Basisklasse
};
// size 0x8
struct bTList_ScrollerDatum_ {
    struct bList base; // +0x0  Basisklasse
};
// size 0x1C
struct Scrollerina {
    struct bTList_ScrollerSlot_ Slots; // +0x0
    struct bTList_ScrollerDatum_ Data; // +0x8
    struct ScrollerDatum *SelectedDatum; // +0x10
    struct ScrollerDatum *TopDatum; // +0x14
    struct ScrollerSlot *SelectedSlot; // +0x18
};
// size 0x20
struct ScrollyLogic {
    int mnNumVisuals; // +0x0
    int mnMaxItems; // +0x4
    int mnItems; // +0x8
    int mnNumVisible; // +0xC
    int mnActive; // +0x10
    int mnCurSel; // +0x14
    int mnTop; // +0x18
    int mnBottom; // +0x1C
};
// size 0x28
struct SemiWheelMover {
    struct Mover base; // +0x0  Basisklasse
    struct bVector3 AxisOffset; // +0x14
    float CurrentAngle; // +0x24
};
// size 0x8
struct bTNode_SetupStats_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0xC
struct SetupStats {
    struct bTNode_SetupStats_ base; // +0x0  Basisklasse
    int NumCombinations; // +0x8
};
// size 0x10
struct SkeletonData {
    struct AnimationTarget base; // +0x0  Basisklasse
    unsigned short mCoreSkelCheckSum; // +0x4
    unsigned short mFlags; // +0x6
    int mNumBones; // +0x8
    float *mInvBoneScales; // +0xC
};
// size 0x10
struct Skeleton {
    struct SkeletonData base; // +0x0  Basisklasse
};
// size 0x4
struct SkidMaker {
    struct SkidSet *pSkidSet; // +0x0
};
// size 0x10
struct SkidSegment {
    float Position[3]; // +0x0
    signed char DeltaPosition[3]; // +0xC
    unsigned char Intensity; // +0xF
};
// size 0x8
struct bTNode_SkidSet_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0xF8
struct SkidSet {
    struct bTNode_SkidSet_ base; // +0x0  Basisklasse
    struct bVector3 LastNormal; // +0x8
    float LastSegmentLength; // +0x18
    struct Car *pCar; // +0x1C
    int bCarVisibleInPlayerViews; // +0x20
    struct Clan *pClan; // +0x24
    struct bPNode *pClanNode; // +0x28
    struct SkidMaker *pSkidMaker; // +0x2C
    int TheTerrainType; // +0x30
    struct bVector3 Position; // +0x34
    struct bVector3 BBoxMax; // +0x44
    struct bVector3 BBoxMin; // +0x54
    struct SkidSegment SkidSegments[8]; // +0x64
    int NumSkidSegments; // +0xE4
    struct bVector3 BBoxCentre; // +0xE8
};
// size 0x8
struct bTNode_SlotPool_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0x4
struct SlotPoolEntry {
    struct SlotPoolEntry *Next; // +0x0
};
// size 0x34
struct SlotPool {
    struct bTNode_SlotPool_ base; // +0x0  Basisklasse
    struct SlotPool *NextSlotPool; // +0x8
    const char *DebugName; // +0xC
    struct SlotPoolEntry *FreeSlots; // +0x10
    enum SlotPoolFlags Flags; // +0x14
    int NumAllocatedSlots; // +0x18
    int MostNumAllocatedSlots; // +0x1C
    int MemoryPool; // +0x20
    int NumSlots; // +0x24
    int SlotSize; // +0x28
    int TotalNumSlots; // +0x2C
    struct SlotPoolEntry Slots[1]; // +0x30
};
// size 0x8
struct bTList_SlotPool_ {
    struct bList base; // +0x0  Basisklasse
};
// size 0xC
struct SlotPoolManager {
    int Initialized; // +0x0
    struct bTList_SlotPool_ SlotPoolList; // +0x4
};
// size 0x1
struct SmokeableInfo {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x1
struct SoundObject {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x4
struct SpaceCase {
    struct SpaceNode *mpSpaceNode; // +0x0
};
// size 0x10
struct SpaceCadet {
    struct SpaceCase base; // +0x0  Basisklasse
    struct Mover *mpMover; // +0x4
    enum eMoverType mMovementMode; // +0x8
    enum eMovementPhase mMovementPhase; // +0xC
};
// size 0x8
struct bTNode_SpaceNode_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0x8
struct bTList_SpaceNode_ {
    struct bList base; // +0x0  Basisklasse
};
// size 0xF0
struct SpaceNode {
    struct bTNode_SpaceNode_ base; // +0x0  Basisklasse
    struct bTList_SpaceNode_ ChildrenList; // +0x8
    struct bMatrix4 WorldMatrix; // +0x10
    struct bMatrix4 LocalMatrix; // +0x50
    struct bVector3 WorldVelocity; // +0x90
    struct bVector3 LocalVelocity; // +0xA0
    struct bVector3 WorldAngularVelocity; // +0xB0
    struct bVector3 LocalAngularVelocity; // +0xC0
    unsigned int NameHash; // +0xD0
    const char *Name; // +0xD4
    unsigned int ReferenceCount; // +0xD8
    unsigned int LastUpdateTimeStamp; // +0xDC
    struct SpaceNode *Parent; // +0xE0
    unsigned int Dirty; // +0xE4
    struct bMatrix4 *BlendingMatrices; // +0xE8
    unsigned int pad1; // +0xEC
};
// size 0x30
struct Spline {
    int ControlPointBufferSize; // +0x0
    int NumControlPoints; // +0x4
    enum SplineType Type; // +0x8
    struct bVector3 *pControlPoints; // +0xC
    struct bVector3 *pControlPointArray; // +0x10
    int Dirty; // +0x14
    int TableSize; // +0x18
    int CalLevel; // +0x1C
    float Length; // +0x20
    float CalTableStep; // +0x24
    int CalTableSize; // +0x28
    float *pCalTable; // +0x2C
};
// size 0x1
struct State {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x4
struct StateTest {
    void **vptr; // +0x0  Vptr (MSVC: am Anfang)
};
// size 0xC
struct StompEntry {
    int nType; // +0x0
    char anon1[4]; // +0x4  anonymer Union/Struct, GC-Größe
    char anon2[4]; // +0x8  anonymer Union/Struct, GC-Größe
};
// size 0xC
struct Stomper {
    bool bEnabled; // +0x0
    char _pad0[0x3]; // +0x1
    char *pStack; // +0x4
    enum eStomper nCurrent; // +0x8
};
// size 0x10
struct SubTitleDesc {
    int numTitles; // +0x0
    char *movieName; // +0x4
    unsigned int *time; // +0x8
    unsigned int *titleHash; // +0xC
};
// size 0x10
struct Symbol {
    const char *name; // +0x0
    const char *type; // +0x4
    void *data; // +0x8
    bool isInternalRef; // +0xC
    char _pad0[0x3]; // +0xD
};
// size 0x4
struct SymbolEntry {
    void *Address; // +0x0
};
// size 0x1
struct System {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x1
struct TAR {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x1
struct TARExtension {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x1
struct TARPrivate {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x8
struct TEXDescriptor {
    TEXHeader *textureHeader; // +0x0
    CLUTHeader *CLUTHeader; // +0x4
};
// size 0x24
struct TEXHeader {
    unsigned short height; // +0x0
    unsigned short width; // +0x2
    unsigned int format; // +0x4
    char *data; // +0x8
    GXTexWrapMode wrapS; // +0xC
    GXTexWrapMode wrapT; // +0x10
    GXTexFilter minFilter; // +0x14
    GXTexFilter magFilter; // +0x18
    float LODBias; // +0x1C
    unsigned char edgeLODEnable; // +0x20
    unsigned char minLOD; // +0x21
    unsigned char maxLOD; // +0x22
    unsigned char unpacked; // +0x23
};
// size 0xC
struct TEXPalette {
    unsigned int versionNumber; // +0x0
    unsigned int numDescriptors; // +0x4
    TEXDescriptor *descriptorArray; // +0x8
};
// size 0x318
struct THREAD {
    int reserved[198]; // +0x0
};
// size 0x10
struct TableBase {
    int NumEntries; // +0x0
    float MinArg; // +0x4
    float MaxArg; // +0x8
    float IndexMultiplier; // +0xC
};
// size 0x14
struct Table {
    struct TableBase base; // +0x0  Basisklasse
    float *pTable; // +0x10
};
// size 0x658
struct TerrainEffectEmitters {
    struct bTList_AcidEmitter_ EmitterList[7][29]; // +0x0
};
// size 0x94
struct TerrainTypeInfo {
    char *Name; // +0x0
    unsigned int NameHash; // +0x4
    enum TerrainType Type; // +0x8
    char ShouldTireMakeRoadSound; // +0xC
    char ShouldCameraGoThroughIt; // +0xD
    char _pad0[0x2]; // +0xE
    char *SkidTextureName; // +0x10
    float SkidWidthMultiplier; // +0x14
    float RoadNoiseGridSpacingInverse; // +0x18
    float RoadNoiseScale; // +0x1C
    float ElevationOffset; // +0x20
    float StaticFrictionMultiplier; // +0x24
    float DynamicFrictionMultiplier; // +0x28
    float RollingFrictionMultiplier; // +0x2C
    float CollisionPriority; // +0x30
    float ForceMagnitude; // +0x34
    float ForceFrequency; // +0x38
    unsigned int DebugTopologyColour; // +0x3C
    const char *DriveOverEmitterGroup; // +0x40
    float DriveOverMinSpeed; // +0x44
    float DriveOverMaxSpeed; // +0x48
    const char *DriveOver2EmitterGroup; // +0x4C
    float DriveOver2MinSpeed; // +0x50
    float DriveOver2MaxSpeed; // +0x54
    const char *SkidSmokeEmitterGroup; // +0x58
    float SkidSmokeMinSpeed; // +0x5C
    float SkidSmokeMaxSpeed; // +0x60
    const char *TireSpewEmitterGroup; // +0x64
    float TireSpewMinSpeed; // +0x68
    float TireSpewMaxSpeed; // +0x6C
    const char *BottomOutEmitterGroup; // +0x70
    float BottomOutMinImpact; // +0x74
    float BottomOutMaxImpact; // +0x78
    const char *CollisionImpactEmitterGroup; // +0x7C
    float CollisionImpactMinForce; // +0x80
    float CollisionImpactMaxForce; // +0x84
    const char *CollisionScrapeEmitterGroup; // +0x88
    float CollisionScrapeMinSpeed; // +0x8C
    float CollisionScrapeMaxSpeed; // +0x90
};
// size 0x14
struct TestMover {
    struct Mover base; // +0x0  Basisklasse
};
// size 0x34
struct TextureAnim {
    char Name[24]; // +0x0
    unsigned int NameHash; // +0x18
    int NumFrames; // +0x1C
    int FramesPerSecond; // +0x20
    int TimeBase; // +0x24
    struct TextureAnimEntry *TextureAnimTable; // +0x28
    int Valid; // +0x2C
    int CurrentFrame; // +0x30
};
// size 0x10
struct TextureAnimEntry {
    unsigned int NameHash; // +0x0
    struct TextureInfo *pTextureInfo; // +0x4
    void *pPlatAnimData; // +0x8
    unsigned int pad; // +0xC
};
// size 0x8
struct bTNode_TextureAnimPack_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0x18
struct TextureAnimPack {
    struct bTNode_TextureAnimPack_ base; // +0x0  Basisklasse
    struct TextureAnim *TextureAnimTable; // +0x8
    struct TextureAnimEntry *TextureAnimEntryTable; // +0xC
    int NumTextureAnims; // +0x10
    int NumTextureAnimEntries; // +0x14
};
// size 0x10
struct TextureAnimPackHeader {
    int Version; // +0x0
    struct TextureAnimPack *pTextureAnimPack; // +0x4
    int EndianSwapped; // +0x8
    int pad; // +0xC
};
// size 0x8
struct TextureIndexEntry {
    unsigned int NameHash; // +0x0
    struct TextureInfo *pTextureInfo; // +0x4
};
// size 0x1
struct TextureInfoPlat {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x8
struct bTNode_TextureInfoPlatInfo_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0xC
struct _GXTlutObj {
    unsigned long dummy[3]; // +0x0
};
// size 0x20
struct _GXTexObj {
    unsigned long dummy[8]; // +0x0
};
// size 0x2C
struct TextureInfoPlatInfoOBJ {
    struct _GXTlutObj objClut; // +0x0
    struct _GXTexObj obj; // +0xC
};
// size 0x3C
struct TextureInfoPlatInfo {
    struct bTNode_TextureInfoPlatInfo_ base; // +0x0  Basisklasse
    struct TextureInfoPlatInfoOBJ ImageInfos; // +0x8
    struct eTextureBucket *pActiveBucket; // +0x34
    unsigned int Format; // +0x38
};
// size 0x4
struct TextureInfoPlatInterface {
    struct TextureInfoPlatInfo *PlatInfo; // +0x0
};
// size 0x8
struct bTNode_TexturePack_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0x84
struct TexturePack {
    struct bTNode_TexturePack_ base; // +0x0  Basisklasse
    char Name[32]; // +0x8
    char Filename[64]; // +0x28
    unsigned int NameHash; // +0x68
    int InventoryHandle; // +0x6C
    struct TexturePackHeader *pPackHeader; // +0x70
    struct TextureInfo *TextureInfoTable; // +0x74
    struct TextureInfoPlatInfo *TextureInfoPlatInfoTable; // +0x78
    int NumTextures; // +0x7C
    int TextureDataSize; // +0x80
};
// size 0x7C
struct TexturePackHeader {
    int Version; // +0x0
    char Name[32]; // +0x4
    char Filename[64]; // +0x24
    unsigned int PermChunkByteOffset; // +0x64
    unsigned int PermChunkByteSize; // +0x68
    int EndianSwapped; // +0x6C
    struct TexturePack *pTexturePack; // +0x70
    struct TextureIndexEntry *TextureIndexEntryTable; // +0x74
    struct eStreamingEntry *TextureStreamEntryTable; // +0x78
};
// size 0x1
struct TextureRenderContext {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x8
struct bTNode_ThumbObject_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0x10
struct ThumbObject {
    struct bTNode_ThumbObject_ base; // +0x0  Basisklasse
    struct FEObject *Object; // +0x8
    struct RecyclingThumbnailNode *ThumbNode; // +0xC
};
// size 0x8
struct bTNode_ThumbnailNode_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0x24 (GC: 0x28)
struct ThumbnailNode {
    struct bTNode_ThumbnailNode_ base; // +0x0  Basisklasse
    unsigned int Item; // +0x8
    struct FEObject *FEngObject; // +0xC
    float XPos; // +0x10
    unsigned int OriginalColor; // +0x14
    bool IsGreyOut; // +0x18
    bool IsFlashable; // +0x19
    char _pad0[0x2]; // +0x1A
    float OrigWidth; // +0x1C
    float OrigHeight; // +0x20
};
// size 0x8
struct bTList_ThumbnailNode_ {
    struct bList base; // +0x0  Basisklasse
};
// size 0x3C
struct ThumbnailScroller {
    void **vptr; // +0x0  Vptr (MSVC: am Anfang)
    struct bTList_ThumbnailNode_ NodeList; // +0x4
    struct ThumbnailNode *CurrentNode; // +0xC
    struct tCubic1D *AnimateCubic; // +0x10
    float SelectedX; // +0x14
    float SelectedY; // +0x18
    float CurrentAddPos; // +0x1C
    const char *PackageName; // +0x20
    const char *GroupName; // +0x24
    struct FEObject *MasterObject; // +0x28
    float MasterWidth; // +0x2C
    float MasterHeight; // +0x30
    struct FEObject *CurrentAlloc; // +0x34
    float PulseState; // +0x38
};
// size 0x8
struct bVector2 {
    float x; // +0x0
    float y; // +0x4
};
// size 0x2C0
struct TopologyCoordinate {
    struct bVector2 InterestBBoxMin; // +0x0
    struct bVector2 InterestBBoxMax; // +0x8
    struct bVector2 CachedBBoxMin; // +0x10
    struct bVector2 CachedBBoxMax; // +0x18
    struct TrackPolygon *TrackPolygonTable[160]; // +0x20
    int TotalNumTrackPolygons; // +0x2A0
    int NumTrackPolygons; // +0x2A4
    int NumVerticalTrackPolygons; // +0x2A8
    int NumUpsideDownTrackPolygons; // +0x2AC
    struct TrackPolygon *pPreviousTrackPolygon; // +0x2B0
    unsigned int pCaffeineObjInterestBBox; // +0x2B4
    unsigned int pCaffeineObjCachedBBox; // +0x2B8
    unsigned int pCaffeineObjPosition; // +0x2BC
};
// size 0x10
struct TopologyEntry {
    enum TerrainType Type; // +0x0
    float Lat; // +0x4
    float Elevation; // +0x8
    float Slope; // +0xC
};
// size 0x8
struct bTList_TrackPolygon_ {
    struct bList base; // +0x0  Basisklasse
};
// size 0x4C
struct TopologyTree {
    struct TopologyTreeNode *pNodeTable; // +0x0
    int NumNodes; // +0x4
    struct TopologyTreeSquare *pSquareTable; // +0x8
    int NumSquares; // +0xC
    struct TopologyTreePolygon *pPolygonTable; // +0x10
    int NumPolygons; // +0x14
    unsigned short *pPolygonIndexTable; // +0x18
    int NumPolygonIndicies; // +0x1C
    struct LString *pPolygonNameTable; // +0x20
    struct bVector3 ExtentsMin; // +0x24
    struct bVector3 ExtentsMax; // +0x34
    struct bTList_TrackPolygon_ TrackPolygonList; // +0x44
};
// size 0x70
struct TopologyTreeFilePolygon {
    struct bVector3 Vertices[4]; // +0x0
    enum TerrainType Type; // +0x40
    unsigned int TextureNameHash; // +0x44
    short NumVertices; // +0x48
    short ScenerySectionNumber; // +0x4A
    short IsVertical; // +0x4C
    short IsUpsideDown; // +0x4E
    char DebugName[32]; // +0x50
};
// size 0x8
struct TopologyTreeNode {
    unsigned short ChildrenNodeNumbers[4]; // +0x0
};
// size 0x20
struct TopologyTreePolygon {
    char UsedByNeighbourFlag; // +0x0
    unsigned char MaxMinBits; // +0x1
    unsigned char Type; // +0x2
    unsigned char Flags; // +0x3
    short ZBase; // +0x4
    short TrackPolygonNumber; // +0x6
    short VerticesX[4]; // +0x8
    short VerticesY[4]; // +0x10
    short VerticesZ[4]; // +0x18
};
// size 0x10
struct TopologyTreeSquare {
    struct bVector2 Center; // +0x0
    int FirstPolygonIndex; // +0x8
    int NumPolygons; // +0xC
};
// size 0x8
struct TrackHeader {
    float TrackLength; // +0x0
    int StartingTroughNumber; // +0x4
};
// size 0x8
struct bTNode_TrackPolygon_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0x74
struct TrackPolygon {
    struct bTNode_TrackPolygon_ base; // +0x0  Basisklasse
    char ReferenceCount; // +0x8
    char NumVertices; // +0x9
    char Flags; // +0xA
    char TheTerrainType; // +0xB
    struct TopologyTreePolygon *pPolygon; // +0xC
    struct TrackPolygonCollisionBody *pCollisionBody; // +0x10
    struct bVector2 BBoxMin; // +0x14
    struct bVector2 BBoxMax; // +0x1C
    struct bVector3 Vertices[4]; // +0x24
    float Normal[3]; // +0x64
    float PlaneDistance; // +0x70
};
// size 0x1
struct TrackPolygonCollisionBody {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x8
struct bTNode_TrackPositionMarker_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0x30
struct TrackPositionMarker {
    struct bTNode_TrackPositionMarker_ base; // +0x0  Basisklasse
    unsigned int NameHash; // +0x8
    int Param; // +0xC
    struct bVector3 Position; // +0x10
    unsigned short Angle; // +0x20
    short Padding0; // +0x22
    int Padding1; // +0x24
    int Padding2; // +0x28
    int Padding3; // +0x2C
};
// size 0x1
struct TrackRoutePoint {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x10
struct TrainInfo {
    float Speed; // +0x0
    int dummy; // +0x4
    int dummy2; // +0x8
    int dummy3; // +0xC
};
// size 0x40
struct Transform {
    union MATRIX4 m; // +0x0
};
// size 0x14
struct ValidJoystickConfiguration {
    int JoystickType; // +0x0
    char ConfigurationNumbers[10]; // +0x4
    unsigned char SteerHysterysis; // +0xE
    unsigned char SteerDeadZoneSize; // +0xF
    unsigned char ThrottleHysterysis; // +0x10
    unsigned char ThrottleDeadZoneSize; // +0x11
    unsigned char BrakeHysterysis; // +0x12
    unsigned char BrakeDeadZoneSize; // +0x13
};
// size 0xC
struct Vec {
    float x; // +0x0
    float y; // +0x4
    float z; // +0x8
};
// size 0xC
struct VerbosityControl {
    int mBaseVerbosity; // +0x0
    int mDefaultBaseVerbosity; // +0x4
    int mInitialized; // +0x8
};
// size 0x1
struct ViewPort {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x1
struct VisibleData {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x60
struct WeatherQuery {
    float Rain; // +0x0
    float Fog; // +0x4
    float Snow; // +0x8
    float Wind; // +0xC
    unsigned int FogColour; // +0x10
    float FogStart; // +0x14
    float RootScale; // +0x18
    float HitScale; // +0x1C
    const char *RegionR; // +0x20
    unsigned int RegionR_Colour; // +0x24
    float RegionR_FogStart; // +0x28
    float RegionR_Scale; // +0x2C
    const char *Region0; // +0x30
    unsigned int Region0_Colour; // +0x34
    float Region0_FogStart; // +0x38
    float Region0_Scale; // +0x3C
    const char *Region1; // +0x40
    unsigned int Region1_Colour; // +0x44
    float Region1_FogStart; // +0x48
    float Region1_Scale; // +0x4C
    const char *Region2; // +0x50
    unsigned int Region2_Colour; // +0x54
    float Region2_FogStart; // +0x58
    float Region2_Scale; // +0x5C
};
// size 0x8
struct bTNode_WeatherRegion_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0xE8
struct WeatherRegion {
    struct bTNode_WeatherRegion_ base; // +0x0  Basisklasse
    char Name[32]; // +0x8
    char GroupName[32]; // +0x28
    int Type; // +0x48
    unsigned int FogColour; // +0x4C
    float FogStart; // +0x50
    float Parameter1; // +0x54
    float PositionX; // +0x58
    float PositionY; // +0x5C
    float PositionZ; // +0x60
    float Radius; // +0x64
    float Intensity; // +0x68
    float FarFalloffStart; // +0x6C
    float AABBmaxX; // +0x70
    float AABBmaxY; // +0x74
    float AABBmaxZ; // +0x78
    float AABBminX; // +0x7C
    float AABBminY; // +0x80
    float AABBminZ; // +0x84
    float FogFalloff; // +0x88
    float FogFalloffX; // +0x8C
    float FogFalloffY; // +0x90
    float padFloat; // +0x94
    unsigned int NameHash; // +0x98
    unsigned int GroupNameHash; // +0x9C
    int Shape; // +0xA0
    int Blend; // +0xA4
    float m00; // +0xA8
    float m01; // +0xAC
    float m02; // +0xB0
    float m03; // +0xB4
    float m10; // +0xB8
    float m11; // +0xBC
    float m12; // +0xC0
    float m13; // +0xC4
    float m20; // +0xC8
    float m21; // +0xCC
    float m22; // +0xD0
    float m23; // +0xD4
    float m30; // +0xD8
    float m31; // +0xDC
    float m32; // +0xE0
    float m33; // +0xE4
};
// size 0x8
struct bTNode_WeatherRegionPack_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0x10
struct WeatherRegionPack {
    struct bTNode_WeatherRegionPack_ base; // +0x0  Basisklasse
    int Version; // +0x8
    int NumRegions; // +0xC
};
// size 0x8
struct bTNode_WorldEntity_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0x1C
struct WorldEntity {
    void **vptr; // +0x0  Vptr (MSVC: am Anfang)
    struct bTNode_WorldEntity_ base; // +0x4  Basisklasse
    int mTypeHash; // +0xC
    int mNameHash; // +0x10
    int mIDNumber; // +0x14
    struct WorldEntityType *mpWET; // +0x18
};
// size 0x1
struct WorldEntityList {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x1
struct WorldEntityRegistration {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x8
struct bTNode_WorldEntityType_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0x8
struct bTList_WorldEntityType_ {
    struct bList base; // +0x0  Basisklasse
};
// size 0x8
struct bTList_WorldEntity_ {
    struct bList base; // +0x0  Basisklasse
};
// size 0x34
struct WorldEntityType {
    struct bTNode_WorldEntityType_ base; // +0x0  Basisklasse
    struct WorldEntity * (* mpSpawnFunction)(int, int); // +0x8
    char *mTypeName; // +0xC
    unsigned int mTypeHash; // +0x10
    unsigned int mParentTypeHash; // +0x14
    bool mAutoListing; // +0x18
    char _pad0[0x3]; // +0x19
    int mMasterIDNumber; // +0x1C
    struct bTList_WorldEntityType_ ChildTypeList; // +0x20
    struct WorldEntityType *mpParentType; // +0x28
    struct bTList_WorldEntity_ WorldEntityList; // +0x2C
};
// size 0x8
struct bTNode_WorldModel_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0x64
struct WorldModel {
    struct bTNode_WorldModel_ base; // +0x0  Basisklasse
    struct eModel *pModel; // +0x8
    struct eModel *pReflectionModel; // +0xC
    int Enabled; // +0x10
    int RenderInSplitScreen; // +0x14
    int mInvisibleInside; // +0x18
    bool AddLighting; // +0x1C
    char _pad0[0x3]; // +0x1D
    struct bMatrix4 Matrix; // +0x20
    struct SpaceNode *pSpaceNode; // +0x60
};
// size 0x20
struct WorldObjectAnimationInfo {
    unsigned int NameHash[3]; // +0x0
    unsigned int AnimSkeletalName; // +0xC
    unsigned int AnimSkeletalSkel; // +0x10
    float AnimSpeed; // +0x14
    int AnimPlayControl; // +0x18
    float AnimDelayTime; // +0x1C
};
// size 0x1
struct WorldObjectCollisionBody {
    char _empty; // +0x0 (leere Klasse)
};
// size 0xA0
struct WorldObjectInfo {
    unsigned int TypeHash; // +0x0
    unsigned int NameHash; // +0x4
    unsigned int CollisionVolumeHash; // +0x8
    unsigned int Cohort; // +0xC
    struct bMatrix4 Matrix; // +0x10
    unsigned int LODNameHash[4]; // +0x50
    unsigned short FarClipSize[4]; // +0x60
    unsigned int Driveable; // +0x68
    unsigned int SoundHash; // +0x6C
    unsigned int ParentNameHash; // +0x70
    unsigned int IsAvoidable; // +0x74
    unsigned int dummy2; // +0x78
    unsigned int dummy3; // +0x7C
    struct WorldObjectAnimationInfo AnimInfo; // +0x80
};
// size 0x24
struct _AXPBMIX {
    unsigned short vL; // +0x0
    unsigned short vDeltaL; // +0x2
    unsigned short vR; // +0x4
    unsigned short vDeltaR; // +0x6
    unsigned short vAuxAL; // +0x8
    unsigned short vDeltaAuxAL; // +0xA
    unsigned short vAuxAR; // +0xC
    unsigned short vDeltaAuxAR; // +0xE
    unsigned short vAuxBL; // +0x10
    unsigned short vDeltaAuxBL; // +0x12
    unsigned short vAuxBR; // +0x14
    unsigned short vDeltaAuxBR; // +0x16
    unsigned short vAuxBS; // +0x18
    unsigned short vDeltaAuxBS; // +0x1A
    unsigned short vS; // +0x1C
    unsigned short vDeltaS; // +0x1E
    unsigned short vAuxAS; // +0x20
    unsigned short vDeltaAuxAS; // +0x22
};
// size 0xE
struct _AXPBITD {
    unsigned short flag; // +0x0
    unsigned short bufferHi; // +0x2
    unsigned short bufferLo; // +0x4
    unsigned short shiftL; // +0x6
    unsigned short shiftR; // +0x8
    unsigned short targetShiftL; // +0xA
    unsigned short targetShiftR; // +0xC
};
// size 0xE
struct _AXPBUPDATE {
    unsigned short updNum[5]; // +0x0
    unsigned short dataHi; // +0xA
    unsigned short dataLo; // +0xC
};
// size 0x12
struct _AXPBDPOP {
    short aL; // +0x0
    short aAuxAL; // +0x2
    short aAuxBL; // +0x4
    short aR; // +0x6
    short aAuxAR; // +0x8
    short aAuxBR; // +0xA
    short aS; // +0xC
    short aAuxAS; // +0xE
    short aAuxBS; // +0x10
};
// size 0x4
struct _AXPBVE {
    unsigned short currentVolume; // +0x0
    short currentDelta; // +0x2
};
// size 0x6
struct _AXPBFIR {
    unsigned short numCoefs; // +0x0
    unsigned short coefsHi; // +0x2
    unsigned short coefsLo; // +0x4
};
// size 0x10
struct _AXPBADDR {
    unsigned short loopFlag; // +0x0
    unsigned short format; // +0x2
    unsigned short loopAddressHi; // +0x4
    unsigned short loopAddressLo; // +0x6
    unsigned short endAddressHi; // +0x8
    unsigned short endAddressLo; // +0xA
    unsigned short currentAddressHi; // +0xC
    unsigned short currentAddressLo; // +0xE
};
// size 0x28
struct _AXPBADPCM {
    unsigned short a[8][2]; // +0x0
    unsigned short gain; // +0x20
    unsigned short pred_scale; // +0x22
    unsigned short yn1; // +0x24
    unsigned short yn2; // +0x26
};
// size 0xE
struct _AXPBSRC {
    unsigned short ratioHi; // +0x0
    unsigned short ratioLo; // +0x2
    unsigned short currentAddressFrac; // +0x4
    unsigned short last_samples[4]; // +0x6
};
// size 0x6
struct _AXPBADPCMLOOP {
    unsigned short loop_pred_scale; // +0x0
    unsigned short loop_yn1; // +0x2
    unsigned short loop_yn2; // +0x4
};
// size 0x8
struct _AXPBLPF {
    unsigned short on; // +0x0
    unsigned short yn1; // +0x2
    unsigned short a0; // +0x4
    unsigned short b0; // +0x6
};
// size 0xF4
struct _AXPB {
    unsigned short nextHi; // +0x0
    unsigned short nextLo; // +0x2
    unsigned short currHi; // +0x4
    unsigned short currLo; // +0x6
    unsigned short srcSelect; // +0x8
    unsigned short coefSelect; // +0xA
    unsigned short mixerCtrl; // +0xC
    unsigned short state; // +0xE
    unsigned short type; // +0x10
    AXPBMIX mix; // +0x12
    AXPBITD itd; // +0x36
    AXPBUPDATE update; // +0x44
    AXPBDPOP dpop; // +0x52
    AXPBVE ve; // +0x64
    AXPBFIR fir; // +0x68
    AXPBADDR addr; // +0x6E
    AXPBADPCM adpcm; // +0x7E
    AXPBSRC src; // +0xA6
    AXPBADPCMLOOP adpcmLoop; // +0xB4
    AXPBLPF lpf; // +0xBA
    unsigned short pad[25]; // +0xC2
};
// size 0x40
struct _AXPBITDBUFFER {
    short data[32]; // +0x0
};
// size 0x100
struct _AXPBU {
    unsigned short data[128]; // +0x0
};
// size 0x38
struct _AXPROFILE {
    long long axFrameStart; // +0x0
    long long auxProcessingStart; // +0x8
    long long auxProcessingEnd; // +0x10
    long long userCallbackStart; // +0x18
    long long userCallbackEnd; // +0x20
    long long axFrameEnd; // +0x28
    unsigned int axNumVoices; // +0x30
    unsigned int __padding; // +0x34
};
// size 0x36
struct _AXSPB {
    unsigned short dpopLHi; // +0x0
    unsigned short dpopLLo; // +0x2
    short dpopLDelta; // +0x4
    unsigned short dpopRHi; // +0x6
    unsigned short dpopRLo; // +0x8
    short dpopRDelta; // +0xA
    unsigned short dpopSHi; // +0xC
    unsigned short dpopSLo; // +0xE
    short dpopSDelta; // +0x10
    unsigned short dpopALHi; // +0x12
    unsigned short dpopALLo; // +0x14
    short dpopALDelta; // +0x16
    unsigned short dpopARHi; // +0x18
    unsigned short dpopARLo; // +0x1A
    short dpopARDelta; // +0x1C
    unsigned short dpopASHi; // +0x1E
    unsigned short dpopASLo; // +0x20
    short dpopASDelta; // +0x22
    unsigned short dpopBLHi; // +0x24
    unsigned short dpopBLLo; // +0x26
    short dpopBLDelta; // +0x28
    unsigned short dpopBRHi; // +0x2A
    unsigned short dpopBRLo; // +0x2C
    short dpopBRDelta; // +0x2E
    unsigned short dpopBSHi; // +0x30
    unsigned short dpopBSLo; // +0x32
    short dpopBSDelta; // +0x34
};
// size 0x22C
struct _AXVPB {
    void *next; // +0x0
    void *prev; // +0x4
    void *next1; // +0x8
    unsigned int priority; // +0xC
    void (* callback)(void *); // +0x10
    unsigned int userContext; // +0x14
    unsigned int index; // +0x18
    unsigned int sync; // +0x1C
    unsigned int depop; // +0x20
    unsigned int updateMS; // +0x24
    unsigned int updateCounter; // +0x28
    unsigned int updateTotal; // +0x2C
    unsigned short *updateWrite; // +0x30
    unsigned short updateData[128]; // +0x34
    void *itdBuffer; // +0x134
    AXPB pb; // +0x138
};
// size 0x18
struct _Bigint {
    struct _Bigint *_next; // +0x0
    int _k; // +0x4
    int _maxwds; // +0x8
    int _sign; // +0xC
    int _wds; // +0x10
    unsigned int _x[1]; // +0x14
};
// size 0x4
struct _GXColor {
    unsigned char r; // +0x0
    unsigned char g; // +0x1
    unsigned char b; // +0x2
    unsigned char a; // +0x3
};
// size 0x8
struct _GXColorS10 {
    short r; // +0x0
    short g; // +0x2
    short b; // +0x4
    short a; // +0x6
};
// size 0x14
struct _GXFogAdjTable {
    unsigned short r[10]; // +0x0
};
// size 0x40
struct _GXLightObj {
    unsigned long dummy[16]; // +0x0
};
// size 0x3C
struct _GXRenderModeObj {
    VITVMode viTVmode; // +0x0
    unsigned short fbWidth; // +0x4
    unsigned short efbHeight; // +0x6
    unsigned short xfbHeight; // +0x8
    unsigned short viXOrigin; // +0xA
    unsigned short viYOrigin; // +0xC
    unsigned short viWidth; // +0xE
    unsigned short viHeight; // +0x10
    char _pad0[0x2]; // +0x12
    VIXFBMode xFBmode; // +0x14
    unsigned char field_rendering; // +0x18
    unsigned char aa; // +0x19
    unsigned char sample_pattern[12][2]; // +0x1A
    unsigned char vfilter[7]; // +0x32
    char _pad1[0x3]; // +0x39
};
// size 0x10
struct _GXTexRegion {
    unsigned long dummy[4]; // +0x0
};
// size 0x10
struct _GXTlutRegion {
    unsigned long dummy[4]; // +0x0
};
// size 0x10
struct _GXVtxAttrFmtList {
    GXAttr attr; // +0x0
    GXCompCnt cnt; // +0x4
    GXCompType type; // +0x8
    unsigned char frac; // +0xC
    char _pad0[0x3]; // +0xD
};
// size 0x8
struct _GXVtxDescList {
    GXAttr attr; // +0x0
    GXAttrType type; // +0x4
};
// size 0x8
union __dmath {
    unsigned int i[2]; // +0x0
    double d; // +0x0
};
// size 0x28
struct __exception {
    int type; // +0x0
    char *name; // +0x4
    double arg1; // +0x8
    double arg2; // +0x10
    double retval; // +0x18
    int err; // +0x20
    char _pad0[0x4]; // +0x24
};
// size 0x10
struct __fmath_type {
    float x; // +0x0
    float y; // +0x4
    float z; // +0x8
    float w; // +0xC
};
// size 0x8
struct __sbuf {
    unsigned char *_base; // +0x0
    int _size; // +0x4
};
// size 0x60
struct __sFILE {
    unsigned char *_p; // +0x0
    int _r; // +0x4
    int _w; // +0x8
    short _flags; // +0xC
    short _file; // +0xE
    struct __sbuf _bf; // +0x10
    int _lbfsize; // +0x18
    void *_cookie; // +0x1C
    int (* _read)(void *, char *, int); // +0x20
    int (* _write)(void *, const char *, int); // +0x24
    int (* _seek)(void *, long, int); // +0x28
    int (* _close)(void *); // +0x2C
    struct __sbuf _ub; // +0x30
    unsigned char *_up; // +0x38
    int _ur; // +0x3C
    unsigned char _ubuf[10]; // +0x40
    unsigned char _nbuf[1]; // +0x4A
    char _pad0[0x1]; // +0x4B
    struct __sbuf _lb; // +0x4C
    int _blksize; // +0x54
    int _offset; // +0x58
    struct _reent *_data; // +0x5C
};
// size 0xC
struct __va_list_tag {
    char gpr; // +0x0
    char fpr; // +0x1
    char _pad0[0x2]; // +0x2
    char *overflow_arg_area; // +0x4
    char *reg_save_area; // +0x8
};
// size 0x60
struct __va_regsave_t {
    long __gp_save[8]; // +0x0
    double __fp_save[8]; // +0x20
};
// size 0xC (GC: 0x8)
struct __vtbl_ptr_type {
    short __delta; // +0x0
    short __index; // +0x2
    void *__pfn; // +0x4
    short __delta2; // +0x8
    char _pad0[0x2]; // +0xA
};
// size 0x8 (GC: 0x88)
struct _atexit {
    struct _atexit *_next; // +0x0
    int _ind; // +0x4
};
// size 0xC
struct _glue {
    struct _glue *_next; // +0x0
    int _niobs; // +0x4
    struct __sFILE *_iobs; // +0x8
};
// size 0x280 (GC: 0x304)
struct _reent {
    int _errno; // +0x0
    struct __sFILE *_stdin; // +0x4
    struct __sFILE *_stdout; // +0x8
    struct __sFILE *_stderr; // +0xC
    int _inc; // +0x10
    char _emergency[25]; // +0x14
    char _pad0[0x3]; // +0x2D
    int _current_category; // +0x30
    const char *_current_locale; // +0x34
    int __sdidinit; // +0x38
    void (* __cleanup)(struct _reent *); // +0x3C
    struct _Bigint *_result; // +0x40
    int _result_k; // +0x44
    struct _Bigint *_p5s; // +0x48
    struct _Bigint **_freelist; // +0x4C
    int _cvtlen; // +0x50
    char *_cvtbuf; // +0x54
    char _new[240]; // +0x58  anonymer Union/Struct, GC-Größe
    struct _atexit *_atexit; // +0x148
    struct _atexit _atexit0; // +0x14C
    struct _glue __sglue; // +0x154
    struct __sFILE __sf[3]; // +0x160
};
// size 0x8
struct _types_fd_set {
    long fds_bits[2]; // +0x0
};
// size 0x8
struct bChunk {
    unsigned int ID; // +0x0
    int Size; // +0x4
};
// size 0xE8
struct bFile {
    int OpenFlags; // +0x0
    int FileSize; // +0x4
    int Position; // +0x8
    int FileHandle; // +0xC
    struct CachedRealFileHandle *pCachedRealFileHandle; // +0x10
    int CloseAfterCallbacks; // +0x14
    struct bFileDirectoryEntry *pDirectoryEntry; // +0x18
    int NumPendingCallbacks; // +0x1C
    char Filename[200]; // +0x20
};
// size 0x8
struct bTNode_bFileCallbackEntry_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0x18
struct bFileCallbackEntry {
    struct bTNode_bFileCallbackEntry_ base; // +0x0  Basisklasse
    struct bFile *File; // +0x8
    void (* Callback)(struct bFile *, int); // +0xC
    int CallbackParam; // +0x10
    int Fop; // +0x14
};
// size 0x4
struct bFileClosePacket {
    int FileID; // +0x0
};
// size 0x18
struct bFileDirectoryEntry {
    unsigned int Hash; // +0x0
    int FileNumber; // +0x4
    int LocalSectorOffset; // +0x8
    int TotalSectorOffset; // +0xC
    int Size; // +0x10
    unsigned int Checksum; // +0x14
};
// size 0xDC
struct bFileOpenPacket {
    char OpenFlags; // +0x0
    char CDInDrive; // +0x1
    char WarnIfCantOpen; // +0x2
    char FutureExpansion; // +0x3
    int FileID; // +0x4
    int FilePositionAddress; // +0x8
    unsigned int CDChecksum; // +0xC
    int CDSize; // +0x10
    char Filename[200]; // +0x14
};
// size 0x10
struct bFileReadPacket {
    int FileID; // +0x0
    int Position; // +0x4
    int Address; // +0x8
    int NumBytes; // +0xC
};
// size 0x7E0
struct bFileWritePacket {
    int FileID; // +0x0
    int Position; // +0x4
    unsigned char Data[2008]; // +0x8
};
// size 0x8
struct bFixVector2 {
    int x; // +0x0
    int y; // +0x4
};
// size 0xC
struct bFixVector3 {
    int x; // +0x0
    int y; // +0x4
    int z; // +0x8
};
// size 0x88
struct bFunkCodeineAddServerPacket {
    struct bFunkPacket *pReceivePackets; // +0x0
    int MaxReceivePackets; // +0x4
    char Name[128]; // +0x8
};
// size 0x8
struct bFunkJoylogCaptureInitPacket {
    int BufferAddress; // +0x0
    int BufferSizeAddress; // +0x4
};
// size 0x1C
struct bFunkPacketHeader {
    unsigned short TotalSize; // +0x0
    unsigned char Type; // +0x2
    unsigned char FunctionNum; // +0x3
    unsigned int DestServer; // +0x4
    unsigned int SourceServer; // +0x8
    unsigned short PacketID; // +0xC
    unsigned short Checksum; // +0xE
    unsigned int ReturnBufferAddress; // +0x10
    unsigned int ReturnCodeAddress; // +0x14
    short ReturnCode; // +0x18
    short MaxReturnSize; // +0x1A
};
// size 0x7FC
struct bFunkPacket {
    struct bFunkPacketHeader base; // +0x0  Basisklasse
    unsigned char Data[2016]; // +0x1C
};
// size 0x8
struct bTNode_bFunkServer_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0x2D0
struct bFunkServer {
    void **vptr; // +0x0  Vptr (MSVC: am Anfang)
    struct bTNode_bFunkServer_ base; // +0x4  Basisklasse
    unsigned int NameHash; // +0xC
    char Name[64]; // +0x10
    char FunctionTypes[128]; // +0x50
    void *FunctionTable[128]; // +0xD0
};
// size 0x2E0
struct bFunkServerPlatform {
    struct bFunkServer base; // +0x0  Basisklasse
    int MaxReceivePackets; // +0x2D0
    int NextReceivePacketNum; // +0x2D4
    bool ProcessingPacket; // +0x2D8
    char _pad0[0x3]; // +0x2D9
    struct bFunkPacket *pReceivePackets; // +0x2DC
};
// size 0x44
struct bMemoryTraceAllocatePacket {
    int PoolID; // +0x0
    int MemoryAddress; // +0x4
    int Size; // +0x8
    int DebugLine; // +0xC
    int AllocationNumber; // +0x10
    char DebugText[48]; // +0x14
};
// size 0x4
struct bMemoryTraceDeletePoolPacket {
    int PoolID; // +0x0
};
// size 0xC
struct bMemoryTraceFreePacket {
    int PoolID; // +0x0
    int MemoryAddress; // +0x4
    int Size; // +0x8
};
// size 0x24
struct bMemoryTraceNewPoolPacket {
    int PoolID; // +0x0
    char Name[32]; // +0x4
};
// size 0x8
struct bPList_QuickSpline_ {
    struct bTList_bPNode_ base; // +0x0  Basisklasse
};
// size 0x8
struct bPList_QuickSpline__bTList_bPNode_ {
    struct bList base; // +0x0  Basisklasse
};
// size 0x8
struct bPList_SkidSet__bTList_bPNode_ {
    struct bList base; // +0x0  Basisklasse
};
// size 0x8
struct bTNode_bPNode_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0xC
struct bPNode {
    struct bTNode_bPNode_ base; // +0x0  Basisklasse
    void *Object; // +0x8
};
// size 0x8
struct bPolar {
    float r; // +0x0
    unsigned short a; // +0x4
    char _pad0[0x2]; // +0x6
};
// size 0x10
struct bQuaternion {
    float x; // +0x0
    float y; // +0x4
    float z; // +0x8
    float w; // +0xC
};
// size 0x24
struct bRepeatChannel {
    int RepeatSize; // +0x0
    int DefaultRemoveData; // +0x4
    int AddDataSize; // +0x8
    int AddRepeatCount; // +0xC
    int AddLocation; // +0x10
    int AddData; // +0x14
    int RemoveRepeatCount; // +0x18
    int RemoveLocation; // +0x1C
    int RemoveData; // +0x20
};
// size 0x54
struct bRepeat {
    int Magic; // +0x0
    int CodeineNeedsUpdating; // +0x4
    int AddLocation; // +0x8
    int HeaderSize; // +0xC
    int RemoveLocation; // +0x10
    int AddBufferSize; // +0x14
    int NumChannels; // +0x18
    int EnableCodeineCapturing; // +0x1C
    int NumItemsAdded; // +0x20
    int NumItemsRemoved; // +0x24
    int RemoveDataError; // +0x28
    int RecordingTime; // +0x2C
    struct bRepeatChannel Channel[1]; // +0x30
};
// size 0x1
struct bSpline {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x8
struct bTList_CAnimSkeleton_ {
    struct bList base; // +0x0  Basisklasse
};
// size 0x8
struct bTList_CachedRealFileHandle_ {
    struct bList base; // +0x0  Basisklasse
};
// size 0x8
struct bTList_CameraMover_ {
    struct bList base; // +0x0  Basisklasse
};
// size 0x8
struct bTList_Clan_ {
    struct bList base; // +0x0  Basisklasse
};
// size 0x8
struct bTList_DebugTopology_ {
    struct bList base; // +0x0  Basisklasse
};
// size 0x8
struct bTList_FernFlattener_ {
    struct bList base; // +0x0  Basisklasse
};
// size 0x8
struct bTList_FernShaker_ {
    struct bList base; // +0x0  Basisklasse
};
// size 0x8
struct bTList_Mover_ {
    struct bList base; // +0x0  Basisklasse
};
// size 0x8
struct bTList_QueuedFile_ {
    struct bList base; // +0x0  Basisklasse
};
// size 0x8
struct bTList_SetupStats_ {
    struct bList base; // +0x0  Basisklasse
};
// size 0x8
struct bTList_WeatherRegion_ {
    struct bList base; // +0x0  Basisklasse
};
// size 0x8
struct bTList_WorldEntityList_ {
    struct bList base; // +0x0  Basisklasse
};
// size 0x8
struct bTList_WorldModel_ {
    struct bList base; // +0x0  Basisklasse
};
// size 0x8
struct bTList_bFileCallbackEntry_ {
    struct bList base; // +0x0  Basisklasse
};
// size 0x8
struct bTList_eDynamicLight_ {
    struct bList base; // +0x0  Basisklasse
};
// size 0x8
struct bTList_eLightFlare_ {
    struct bList base; // +0x0  Basisklasse
};
// size 0x8
struct bTList_eModel_ {
    struct bList base; // +0x0  Basisklasse
};
// size 0x8
struct bTList_eSolid_ {
    struct bList base; // +0x0  Basisklasse
};
// size 0x8
struct bTNode_TextureInfo_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0x8
struct bTNode_eDynamicFrameMemory_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0x8
struct bTNode_eDynamicLightPack_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0x8
struct bTNode_eDynamicLight_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0x8
struct bTNode_eLightFlarePackHeader_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0x8
struct bTNode_eLightFlare_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0x8
struct bTNode_eLightMaterial_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0x8
struct bTNode_eLightPack_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0x8
struct bTNode_eModel_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0x8
struct bTNode_ePoly_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0x8
struct bTNode_eSolidListHeader_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0x8
struct bTNode_eSolid_ {
    struct bNode base; // +0x0  Basisklasse
};
// size 0x1
struct bVU0Float {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x1
struct bVector3Reg {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x4
struct bVirtualDestructNode {
    void **vptr; // +0x0  Vptr (MSVC: am Anfang)
};
// size 0x1
struct bad_alloc {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x20
struct cCaptureBuffer {
    void **vptr; // +0x0  Vptr (MSVC: am Anfang)
    struct TextureInfo *pCaptureTexture; // +0x4
    int xOrigin; // +0x8
    int yOrigin; // +0xC
    int width; // +0x10
    int height; // +0x14
    char *pCapturePixels; // +0x18
    unsigned char bInitialized; // +0x1C
    char _pad0[0x3]; // +0x1D
};
// size 0x158 (GC: 0x17C)
struct cDBFilesBase {
    void **vptr; // +0x0  Vptr (MSVC: am Anfang)
    enum eFILE_SYS_STATES mState; // +0x4
    enum eFILE_SYS_STATES mNextState; // +0x8
    enum eFILE_SYS_STATES mCallBackState; // +0xC
    enum eFILE_SYS_SUBSTATES mSubState; // +0x10
    enum eFILE_SYS_SUBSTATES mPrevSubState; // +0x14
    char mFileName[256]; // +0x18
    void *mFileBuffer; // +0x118
    int mFileBufferSize; // +0x11C
    int mNumCards; // +0x120
    int mFreeSpace; // +0x124
    int mLastError; // +0x128
    unsigned int mHash; // +0x12C
    void *mParameter[9]; // +0x130
    float mElapsedTime; // +0x154
};
// size 0x15C (GC: 0x180)
struct cDBFileSys {
    struct cDBFilesBase base; // +0x0  Basisklasse
    bool mFileExists; // +0x158
    char _pad0[0x3]; // +0x159
};
// size 0x1
struct cFEngRender {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x1
struct cMC {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x30
struct tCubic1D {
    float Val; // +0x0
    float dVal; // +0x4
    float ValDesired; // +0x8
    float dValDesired; // +0xC
    float Coeff[4]; // +0x10
    float time; // +0x20
    float duration; // +0x24
    float arrivalThreshold; // +0x28
    short state; // +0x2C
    short flags; // +0x2E
};
// size 0x90
struct tCubic3D {
    struct tCubic1D x; // +0x0
    struct tCubic1D y; // +0x30
    struct tCubic1D z; // +0x60
};
// size 0x198
struct cPoint {
    void **vptr; // +0x0  Vptr (MSVC: am Anfang)
    struct tCubic3D mPos; // +0x4
    struct tCubic3D mRot; // +0x94
    struct tCubic1D mScale; // +0x124
    struct tCubic1D mYScale; // +0x154
    float mZBias; // +0x184
    void (* mCallBackFunc)(int, int); // +0x188
    void (* mUpdateFunc)(struct cPoint *, float); // +0x18C
    int mCBParam1; // +0x190
    int mCBParam2; // +0x194
};
// size 0x1
struct cPostRaceScreenMgrInterface {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x2C
struct cQuarterSizeMap {
    void **vptr; // +0x0  Vptr (MSVC: am Anfang)
    int QUARTER_SIZE_X; // +0x4
    int QUARTER_SIZE_Y; // +0x8
    struct cCaptureBuffer quarterSizeBuffer; // +0xC
};
// size 0x4C
struct cReflectMap {
    void **vptr; // +0x0  Vptr (MSVC: am Anfang)
    int REFLECT_MAP_SIZE_X; // +0x4
    int REFLECT_MAP_SIZE_Y; // +0x8
    struct cCaptureBuffer reflectBuffer[2]; // +0xC
};
// size 0x140
struct cSphereMap {
    void **vptr; // +0x0  Vptr (MSVC: am Anfang)
    struct _GXTexObj *cubeTex[6]; // +0x4
    struct cCaptureBuffer cubeBuffer[6]; // +0x1C
    struct TextureInfo *sphereTexture; // +0xDC
    char *sphereTexturePixels; // +0xE0
    unsigned char bInitialized; // +0xE4
    char _pad0[0x3]; // +0xE5
    void *DLSphere; // +0xE8
    unsigned int DLSphereSz; // +0xEC
    int SPHERE_MAP_SIZE_X; // +0xF0
    int SPHERE_MAP_SIZE_Y; // +0xF4
    enum _GXTexFmt SPHERE_MAP_FMT; // +0xF8
    int SPHERE_MAP_TESS; // +0xFC
    int CUBEFACE_SIZE; // +0x100
    unsigned char CubeFaceStart; // +0x104
    unsigned char CubeFaceEnd; // +0x105
    unsigned char CubeTevMode; // +0x106
    char _pad1[0x1]; // +0x107
    float angle1[6]; // +0x108
    char axis1[6]; // +0x120
    char _pad2[0x2]; // +0x126
    float angle2[6]; // +0x128
};
// size 0x8
struct div_t {
    int quot; // +0x0
    int rem; // +0x4
};
// size 0x1
struct eAnimationSolid {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x1
struct eAnimationTree {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x1
struct eAnimationTreeNode {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x200
struct eDamageInfo {
    float CurrentCells[64]; // +0x0
    float PreviousCells[64]; // +0x100
};
// size 0x2
struct eDamageVertex {
    unsigned short Data; // +0x0
};
// size 0x14
struct eDataRenderDynamic {
    unsigned int *colourtable0; // +0x0
    unsigned int *colourtable1; // +0x4
    Mtx *trm; // +0x8
    Mtx *hm; // +0xC
    Mtx *hcm; // +0x10
};
// size 0x1
struct eDmaChain {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x1
struct eDmaData {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x1
struct eDmaDataList {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x110
struct eDynamicFrameMemory {
    struct bTNode_eDynamicFrameMemory_ base; // +0x0  Basisklasse
    int Size; // +0x8
    int Pad; // +0xC
    char Data[256]; // +0x10
};
// size 0x34
struct eDynamicLightPack {
    struct bTNode_eDynamicLightPack_ base; // +0x0  Basisklasse
    char Name[32]; // +0x8
    int Enabled; // +0x28
    struct bTList_eDynamicLight_ DynamicLightList; // +0x2C
};
// size 0xF00
struct eEnvMap {
    struct eView *Views[6]; // +0x0
    struct Camera Cameras[6]; // +0x18
};
// size 0x10
struct eFilter {
    struct TextureInfo *pTexture; // +0x0
    unsigned char aColour[4]; // +0x4
    unsigned char nAlphaA; // +0x8
    unsigned char nAlphaB; // +0x9
    unsigned char nAlphaC; // +0xA
    unsigned char nAlphaD; // +0xB
    unsigned char bActive; // +0xC
    unsigned char nAlphaFix; // +0xD
    unsigned char nTextureFx; // +0xE
    unsigned char nPadding; // +0xF
};
// size 0x14
struct eFogParams {
    float Start; // +0x0
    float End; // +0x4
    float Intensity; // +0x8
    float FalloffY; // +0xC
    GXColor FogColor; // +0x10
};
// size 0x60
struct eLight {
    unsigned int NameHash; // +0x0
    unsigned char Type; // +0x4
    unsigned char AttenuationType; // +0x5
    unsigned char Shape; // +0x6
    unsigned char State; // +0x7
    unsigned int ExcludeNameHash; // +0x8
    unsigned int Colour; // +0xC
    float PositionX; // +0x10
    float PositionY; // +0x14
    float PositionZ; // +0x18
    float Size; // +0x1C
    float DirectionX; // +0x20
    float DirectionY; // +0x24
    float DirectionZ; // +0x28
    float Intensity; // +0x2C
    float FarStart; // +0x30
    float FarEnd; // +0x34
    float Falloff; // +0x38
    short ScenerySectionNumber; // +0x3C
    char Name[34]; // +0x3E
};
// size 0x1C0
struct eLightContext {
    struct bMatrix4 LocalColourMatrix; // +0x0
    struct bMatrix4 LocalDirectionMatrix; // +0x40
    struct bMatrix4 LocalLightPositions; // +0x80
    struct bVector4 LocalEyePosition; // +0xC0
    int NumLights; // +0xD0
    float SunlightIntensity; // +0xD4
    int pad1; // +0xD8
    int pad2; // +0xDC
    struct bMatrix4 EnvMapMatrix; // +0xE0
    struct bVector4 Harmonics[10]; // +0x120
};
// size 0x50
struct eLightFlare {
    struct bTNode_eLightFlare_ base; // +0x0  Basisklasse
    unsigned int NameHash; // +0x8
    char Enabled; // +0xC
    char Type; // +0xD
    short Flags; // +0xE
    float PositionX; // +0x10
    float PositionY; // +0x14
    float PositionZ; // +0x18
    float ReflectPosZ; // +0x1C
    float DirectionX; // +0x20
    float DirectionY; // +0x24
    float DirectionZ; // +0x28
    short ScenerySectionNumber; // +0x2C
    char Name[34]; // +0x2E
};
// size 0x40
struct eLightFlarePackHeader {
    struct bTNode_eLightFlarePackHeader_ base; // +0x0  Basisklasse
    unsigned int Version; // +0x8
    unsigned int NameHash; // +0xC
    char Name[32]; // +0x10
    unsigned int NumLightFlares; // +0x30
    unsigned int pad; // +0x34
    struct bTList_eLightFlare_ LightFlareList; // +0x38
};
// size 0x70
struct eLightMaterialData {
    float DiffuseMinScale; // +0x0
    float DiffuseMinR; // +0x4
    float DiffuseMinG; // +0x8
    float DiffuseMinB; // +0xC
    float DiffuseMaxScale; // +0x10
    float DiffuseMaxR; // +0x14
    float DiffuseMaxG; // +0x18
    float DiffuseMaxB; // +0x1C
    float DiffuseMinA; // +0x20
    float DiffuseMaxA; // +0x24
    float SpecularPower; // +0x28
    float SpecularMinScale; // +0x2C
    float SpecularMinR; // +0x30
    float SpecularMinG; // +0x34
    float SpecularMinB; // +0x38
    float SpecularMaxScale; // +0x3C
    float SpecularMaxR; // +0x40
    float SpecularMaxG; // +0x44
    float SpecularMaxB; // +0x48
    float EnvmapPower; // +0x4C
    float EnvmapMinScale; // +0x50
    float EnvmapMinR; // +0x54
    float EnvmapMinG; // +0x58
    float EnvmapMinB; // +0x5C
    float EnvmapMaxScale; // +0x60
    float EnvmapMaxR; // +0x64
    float EnvmapMaxG; // +0x68
    float EnvmapMaxB; // +0x6C
};
// size 0x8
struct eLightMaterialEntry {
    unsigned int NameHash; // +0x0
    struct eLightMaterial *LightMaterial; // +0x4
};
// size 0x1
struct eLightMaterialPlatInfo {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x4
struct eLightMaterialPlatInterface {
    struct eLightMaterialPlatInfo *PlatInfo; // +0x0
};
// size 0x1C
struct eLightPack {
    struct bTNode_eLightPack_ base; // +0x0  Basisklasse
    int Version; // +0x8
    struct vAABBTree *LightTree; // +0xC
    int NumTreeNodes; // +0x10
    struct eLight *LightArray; // +0x14
    int NumLights; // +0x18
};
// size 0x18
struct eModel {
    struct bTNode_eModel_ base; // +0x0  Basisklasse
    unsigned int NameHash; // +0x8
    struct eSolid *Solid; // +0xC
    struct eReplacementTextureTable *pReplacementTextureTable; // +0x10
    int NumReplacementTextures; // +0x14
};
// size 0xC
struct eNormalSmoother {
    struct eSmoothVertex *SmoothVertexTable; // +0x0
    struct eSmoothVertexPlat *SmoothVertexPlatTable; // +0x4
    short NumSmoothVertex; // +0x8
    short NumSmoothVertexPlat; // +0xA
};
// size 0x30
struct ePointSprite3D {
    float X; // +0x0
    float Y; // +0x4
    float Z; // +0x8
    float Radius; // +0xC
    float S1; // +0x10
    float T1; // +0x14
    float S0; // +0x18
    float T0; // +0x1C
    unsigned int Colour; // +0x20
    unsigned int pad0; // +0x24
    float Cos; // +0x28
    float Sin; // +0x2C
};
// size 0x90
struct ePoly {
    struct bTNode_ePoly_ base; // +0x0  Basisklasse
    struct bVector3 Vertices[4]; // +0x8
    float UVs[4][3]; // +0x48
    unsigned char Colours[4][4]; // +0x78
    struct eDynamicFrameMemory *pDynamicFrameMemory; // +0x88
    unsigned char ApplyAspectRatio; // +0x8C
    unsigned char Flailer; // +0x8D
    char _pad0[0x2]; // +0x8E
};
// size 0x50
struct ePositionMarker {
    unsigned int NameHash; // +0x0
    int iParam0; // +0x4
    float fParam0; // +0x8
    float fParam1; // +0xC
    struct bMatrix4 Matrix; // +0x10
};
// size 0x44
struct eRenderTarget {
    enum TARGET_ID ID; // +0x0
    const char *Name; // +0x4
    int Active; // +0x8
    int ScissorX; // +0xC
    int ScissorY; // +0x10
    int ScissorW; // +0x14
    int ScissorH; // +0x18
    int FrameAddress; // +0x1C
    int FrameWidth; // +0x20
    int FrameHeight; // +0x24
    int IgnoreAlphaTextures; // +0x28
    enum FILTER_ID CopyFilterID; // +0x2C
    struct _GXColor BackgroundColour; // +0x30
    int ClearBackground; // +0x34
    struct bMatrix4 *WorldClip; // +0x38
    struct bMatrix4 *WorldView; // +0x3C
    struct bMatrix4 *ViewScreen; // +0x40
};
// size 0x10
struct eReplacementTextureTable {
    unsigned int hOldNameHash; // +0x0
    unsigned int hNewNameHash; // +0x4
    struct TextureInfo *pTextureInfo; // +0x8
    struct TextureInfo **ppReplacedTextureInfo; // +0xC
};
// size 0x1
struct eRigidAnimation {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x8
struct eSmoothVertex {
    unsigned int VertexHash; // +0x0
    unsigned char SmoothingGroupNumber; // +0x4
    char NX; // +0x5
    char NY; // +0x6
    char NZ; // +0x7
};
// size 0xC
struct eSmoothVertexPlat {
    unsigned int VertexHash; // +0x0
    unsigned int SmoothingGroup; // +0x4
    unsigned int VertexOffset; // +0x8
};
// size 0x8
struct eSolidIndexEntry {
    unsigned int NameHash; // +0x0
    struct eSolid *Solid; // +0x4
};
// size 0x80
struct eSolidListHeader {
    struct bTNode_eSolidListHeader_ base; // +0x0  Basisklasse
    int Version; // +0x8
    int NumSolids; // +0xC
    char Filename[56]; // +0x10
    char GroupName[32]; // +0x48
    unsigned int PermChunkByteOffset; // +0x68
    unsigned int PermChunkByteSize; // +0x6C
    short MaxSolidChunkByteAlignment; // +0x70
    short EndianSwapped; // +0x72
    int InventoryHandle; // +0x74
    struct eSolidIndexEntry *SolidIndexEntryTable; // +0x78
    struct eStreamingEntry *SolidStreamEntryTable; // +0x7C
};
// size 0x24
struct eSolidPlatInfo {
    unsigned short Version; // +0x0
    unsigned short StripFlags; // +0x2
    unsigned short NumStrips; // +0x4
    unsigned short NumIdxClrTable; // +0x6
    unsigned int SizeofStripData; // +0x8
    unsigned int DataOffset0; // +0xC
    unsigned int DataOffset1; // +0x10
    unsigned int DataOffset2; // +0x14
    unsigned int DataOffset3; // +0x18
    struct eStripEntry *StripEntryTable; // +0x1C
    unsigned char *StripDataStart; // +0x20
};
// size 0x4
struct eSolidPlatInterface {
    struct eSolidPlatInfo *PlatInfo; // +0x0
};
// size 0x1
struct eStreamingEntry {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x1
struct eStripData {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x1
struct eStripDataE {
    struct eStripData base; // +0x0  Basisklasse
};
// size 0x10
struct eStripEntry {
    unsigned int DataOffset; // +0x0
    unsigned short DataSize; // +0x4
    unsigned short Flags; // +0x6
    unsigned char NumVerts; // +0x8
    unsigned char PolyGroupNumber; // +0x9
    char TextureNumber; // +0xA
    char LightMaterialIndex; // +0xB
    unsigned char VertexDescription; // +0xC
    unsigned char VertexFormat; // +0xD
    unsigned short DataDLSize; // +0xE
};
// size 0x1
struct eTextureBucket {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x8
struct eTextureEntry {
    unsigned int NameHash; // +0x0
    struct TextureInfo *pTextureInfo; // +0x4
};
// size 0x1
struct eVertexAnimator {
    char _empty; // +0x0 (leere Klasse)
};
// size 0x4
struct eViewPlatInterface {
    struct eViewPlatInfo *PlatInfo; // +0x0
};
// size 0x54
struct eView {
    struct eViewPlatInterface base; // +0x0  Basisklasse
    enum EVIEW_ID ID; // +0x4
    char Active; // +0x8
    char LetterBox; // +0x9
    char NisPlaying; // +0xA
    char pad1; // +0xB
    float H; // +0xC
    float NearZ; // +0x10
    float FarZ; // +0x14
    float FovBias; // +0x18
    float FovDegrees; // +0x1C
    int BlackAndWhiteMode; // +0x20
    struct bVector3 ViewDirection; // +0x24
    struct Camera *pCamera; // +0x34
    struct bTList_CameraMover_ CameraMoverList; // +0x38
    unsigned int WeatherQueryCounter; // +0x40
    struct TextureInfo *pBlendMask; // +0x44
    struct eLightContext *WorldLightContext; // +0x48
    struct eRenderTarget *RenderTargetTable[2]; // +0x4C
};
// size 0x170 (GC: 0x174)
struct eViewPlatInfo {
    struct bMatrix4 WorldViewMatrix; // +0x0
    struct bMatrix4 ViewScreenMatrix; // +0x40
    struct bMatrix4 WorldClipMatrix; // +0x80
    struct bMatrix4 WorldScreenMatrix; // +0xC0
    struct eView *HeadlightView; // +0x100
    struct eView *ReflectionView; // +0x104
    float aspect; // +0x108
    float fovscl; // +0x10C
    struct bVector4 ClippingPlanes[6]; // +0x110
};
// size 0x18
struct eflock {
    short l_type; // +0x0
    short l_whence; // +0x2
    long l_start; // +0x4
    long l_len; // +0x8
    short l_pid; // +0xC
    short l_xxx; // +0xE
    long l_rpid; // +0x10
    long l_rsys; // +0x14
};
// size 0x10
struct fVector4 {
    struct __fmath_type coord; // +0x0
};
// size 0x40
struct fMatrix4 {
    struct fVector4 v0; // +0x0
    struct fVector4 v1; // +0x10
    struct fVector4 v2; // +0x20
    struct fVector4 v3; // +0x30
};
// size 0x10
struct fVector2 {
    struct __fmath_type coord; // +0x0
};
// size 0x10
struct fVector3 {
    struct __fmath_type coord; // +0x0
};
// size 0x10
struct fVectorW {
    struct __fmath_type coord; // +0x0
};
// size 0x10
struct fVectorX {
    struct __fmath_type coord; // +0x0
};
// size 0x10
struct fVectorY {
    struct __fmath_type coord; // +0x0
};
// size 0x10
struct fVectorZ {
    struct __fmath_type coord; // +0x0
};
// size 0x10
struct flock {
    short l_type; // +0x0
    short l_whence; // +0x2
    long l_start; // +0x4
    long l_len; // +0x8
    short l_pid; // +0xC
    short l_xxx; // +0xE
};
// size 0x8
struct ldiv_t {
    long quot; // +0x0
    long rem; // +0x4
};
// size 0x3C
struct stat {
    short st_dev; // +0x0
    unsigned short st_ino; // +0x2
    unsigned int st_mode; // +0x4
    unsigned short st_nlink; // +0x8
    unsigned short st_uid; // +0xA
    unsigned short st_gid; // +0xC
    short st_rdev; // +0xE
    int st_size; // +0x10
    int st_atime; // +0x14
    long st_spare1; // +0x18
    int st_mtime; // +0x1C
    long st_spare2; // +0x20
    int st_ctime; // +0x24
    long st_spare3; // +0x28
    long st_blksize; // +0x2C
    long st_blocks; // +0x30
    long st_spare4[2]; // +0x34
};
// size 0x60
struct tCubic2D {
    struct tCubic1D x; // +0x0
    struct tCubic1D y; // +0x30
};
// size 0x8
struct tFESettingsFile {
    unsigned int fileSize; // +0x0
    unsigned short palMode; // +0x4
    unsigned short autoSave; // +0x6
};
// size 0x8
struct tGraph_bVector2_ {
    struct GraphEntry_bVector2_ *GraphData; // +0x0
    int NumEntries; // +0x4
};
// size 0x8
struct tGraph_bVector3_ {
    struct GraphEntry_bVector3_ *GraphData; // +0x0
    int NumEntries; // +0x4
};
// size 0x8
struct tGraph_bVector4_ {
    struct GraphEntry_bVector4_ *GraphData; // +0x0
    int NumEntries; // +0x4
};
// size 0x8
struct tGraph_float_ {
    struct GraphEntry_float_ *GraphData; // +0x0
    int NumEntries; // +0x4
};
// size 0x1C
struct tTTFontHeader {
    int mChunkID; // +0x0
    int mChunkSize; // +0x4
    int mHashID; // +0x8
    int mNumGlyphs; // +0xC
    int mNumKerningPairs; // +0x10
    struct tTTGlyph *mGlyph; // +0x14
    struct tTTKerningPair *mKerningPair; // +0x18
};
// size 0x20
struct tTTGlyph {
    short mID; // +0x0
    short mNumKerningPairs; // +0x2
    float mPosX; // +0x4
    float mPosY; // +0x8
    float mWidth; // +0xC
    float mHeight; // +0x10
    float mAdvance; // +0x14
    float mOffset; // +0x18
    struct tTTKerningPair *mKerningTable; // +0x1C
};
// size 0x8
struct tTTKerningPair {
    int mGlyph; // +0x0
    float mXOffset; // +0x4
};
// size 0x14
struct tTable_bVector2_ {
    struct TableBase base; // +0x0  Basisklasse
    struct bVector2 *pTable; // +0x10
};
// size 0x14
struct tTable_bVector3_ {
    struct TableBase base; // +0x0  Basisklasse
    struct bVector3 *pTable; // +0x10
};
// size 0x14
struct tTable_bVector4_ {
    struct TableBase base; // +0x0  Basisklasse
    struct bVector4 *pTable; // +0x10
};
// size 0x14
struct tTable_float_ {
    struct TableBase base; // +0x0  Basisklasse
    float *pTable; // +0x10
};
// size 0x24
struct tm {
    int tm_sec; // +0x0
    int tm_min; // +0x4
    int tm_hour; // +0x8
    int tm_mday; // +0xC
    int tm_mon; // +0x10
    int tm_year; // +0x14
    int tm_wday; // +0x18
    int tm_yday; // +0x1C
    int tm_isdst; // +0x20
};
// size 0x8
union uPPCWGPipe {
    unsigned char u8; // +0x0
    unsigned short u16; // +0x0
    unsigned int u32; // +0x0
    unsigned long long u64; // +0x0
    char s8; // +0x0
    short s16; // +0x0
    int s32; // +0x0
    long long s64; // +0x0
    float f32; // +0x0
    double f64; // +0x0
};
// size 0x30
struct vAABB {
    float PositionX; // +0x0
    float PositionY; // +0x4
    float PositionZ; // +0x8
    short ParentIndex; // +0xC
    short NumChildren; // +0xE
    float ExtentX; // +0x10
    float ExtentY; // +0x14
    float ExtentZ; // +0x18
    short ChildrenIndicies[10]; // +0x1C
};
// size 0x10
struct vAABBTree {
    struct vAABB *NodeArray; // +0x0
    short NumLeafNodes; // +0x4
    short NumParentNodes; // +0x6
    short TotalNodes; // +0x8
    short Depth; // +0xA
    int pad1; // +0xC
};
// size 0x20
struct vSphere {
    float PositionX; // +0x0
    float PositionY; // +0x4
    float PositionZ; // +0x8
    float Radius; // +0xC
    unsigned short ParentIndex; // +0x10
    unsigned short ChildrenIndicies[7]; // +0x12
};
