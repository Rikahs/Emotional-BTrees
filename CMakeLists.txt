cmake_minimum_required (VERSION 3.5)
project (ALMA)
set (CMAKE_CXX_STANDARD 14)

set (PROJECT_DIRECTORY "${CMAKE_SORUCE_DIR}/ALMA")
set (GLM_DIRECTORY "${PROJECT_DIRECTORY}/glm")

include_directories ("${GLM_DIRECTORY}")

set (SSOURCE_FILES
   
    "${PROJECT_DIRECTORY}/Personality/PersonalityEmotionsRelations.cpp"
)
set (HEADER_FILE
    "${PROJECT_DIRECTORY}/Emotions/Emotions.h"
    "${PROJECT_DIRECTORY}/Emotions/EmotionAppraisalVars.h"
    "${PROJECT_DIRECTORY}/Emotions/EmotionHistory.h"
    "${PROJECT_DIRECTORY}/Emotions/EmotionName.h"
    "${PROJECT_DIRECTORY}/Emotions/EmoPADCentroid.h"
    "${PROJECT_DIRECTORY}/Emotions/EmotionsPADRelation.h"
    "${PROJECT_DIRECTORY}/Emotions/EmotionType.h"
    "${PROJECT_DIRECTORY}/Emotions/EmotionVector.h"
    "${PROJECT_DIRECTORY}/Emotions/PADEmotion.h"

    "${PROJECT_DIRECTORY}/Mood/Mood.h"
    "${PROJECT_DIRECTORY}/Mood/MoodType.h"

    "${PROJECT_DIRECTORY}/Personality/Personality.h"
    "${PROJECT_DIRECTORY}/Personality/PersonalityEmotionsRelations.h"
    
    "${PROJECT_DIRECTORY}/Personality/PErsonalityMoodRelations.h"
    
    "${PROJECT_DIRECTORY}/timer_manager.h"

    "${PROJECT_DIRECTORY}/Apprasial/ApprasialVariabes.h"
    "${PROJECT_DIRECTORY}/Apprasial/EEC.h"
    "${PROJECT_DIRECTORY}/Apprasial/eec/Agency.h"
    "${PROJECT_DIRECTORY}/Apprasial/eec/Appealingness.h"
    "${PROJECT_DIRECTORY}/Apprasial/eec/Desirability.h"
    "${PROJECT_DIRECTORY}/Apprasial/eec/Likelihood.h"
    "${PROJECT_DIRECTORY}/Apprasial/eec/Liking.h"
    "${PROJECT_DIRECTORY}/Apprasial/eec/Praiseworthiness.h"
    "${PROJECT_DIRECTORY}/Apprasial/eec/Realization.h"

    "${PROJECT_DIRECTORY}/compute/DecayFunction.h"
    "${PROJECT_DIRECTORY}/compute/EmotionEngine.h"
    "${PROJECT_DIRECTORY}/compute/ExponentialDEcayFunction.h"
    "${PROJECT_DIRECTORY}/compute/LinerDecayFunction.h"
    "${PROJECT_DIRECTORY}/compute/MoodEngine.h"
    "${PROJECT_DIRECTORY}/compute/TangensHyperBolicusDecayFunction.h"

    "${PROJECT_DIRECTORY}/Data/AffectConsts.h"
    "${PROJECT_DIRECTORY}/Manage/AffectManger/h"
    "${PROJECT_DIRECTORY}/Manage/AffectOutputCreaer.h"
    "${PROJECT_DIRECTORY}/Manage/AppraisalManager.h"
    "${PROJECT_DIRECTORY}/Manage/AppraisalRules.h"
    "${PROJECT_DIRECTORY}/Manage/CharacterManager.h"
    "${PROJECT_DIRECTORY}/Manage/DocumentManager.h"
    "${PROJECT_DIRECTORY}/Manage/EntityManager.h"
    "${PROJECT_DIRECTORY}/Manage/GroupManager.h"
    
)

IF(MSVC)
    # make it looks better in msvc
    foreach(source IN LISTS SOURCE_FILES)
        get_filename_component(source_path "${source}" PATH)
        string(REPLACE "/" "\\" source_path_msvc "${source_path}")
        source_group("${source_path_msvc}" FILES "${source}")
    endforeach()
    foreach(header IN LISTS HEADER_FILES)
        get_filename_component(header_path "${header}" PATH)
        string(REPLACE "/" "\\" header_path_msvc "${header_path}")
        source_group("${header_path_msvc}" FILES "${header}")
    endforeach()
ENDIF(MSVC)
include_directories(${PROJECT_DIRECTORY})
add_executable(${CMAKE_PROJECT_NAME} "${SOURCE_FILES} ${HEADER_FILES}")