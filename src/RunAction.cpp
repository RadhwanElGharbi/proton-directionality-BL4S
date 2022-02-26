// -----------------------------------------------------------------------------
//  G4Basic | RunAction.h
//
//
//   * Author: Everybody is an author!
//   * Creation date: 15 Aug 2019
// -----------------------------------------------------------------------------

#include "RunAction.h"
#include "DetectorConstruction.h"

// Q-Pix includes
#include "AnalysisManager.h"
#include "MCTruthManager.h"

// GEANT4 includes
#include "G4Box.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4Run.hh"

// C++ includes
#include <experimental/filesystem>


RunAction::RunAction(): G4UserRunAction(), multirun_(false)
{
    messenger_ = new G4GenericMessenger(this, "/Inputs/");
    messenger_->DeclareProperty("root_output", root_output_path_,
                                "path to output ROOT file");
    messenger_->DeclareProperty("multirun", multirun_,
                                "Multiple runs");
}


RunAction::~RunAction()
{
    delete messenger_;
}


void RunAction::BeginOfRunAction(const G4Run* run)
{
    G4cout << "RunAction::BeginOfRunAction: Run #" << run->GetRunID() << " start." << G4endl;

    std::string root_output_path = root_output_path_;

    if (multirun_)
    {

        std::ostringstream ss;
        ss << std::setw(4) << std::setfill('0') << run->GetRunID();
        std::string run_str(ss.str());

        // G4cout << "run_str: " << run_str << G4endl;

        std::experimental::filesystem::path path = static_cast<std::string> (root_output_path_);

        // G4cout << "root_name:      " << path.root_name()      << G4endl;
        // G4cout << "root_directory: " << path.root_directory() << G4endl;
        // G4cout << "root_path:      " << path.root_path()      << G4endl;
        // G4cout << "relative_path:  " << path.relative_path()  << G4endl;
        // G4cout << "parent_path:    " << path.parent_path()    << G4endl;
        // G4cout << "filename:       " << path.filename()       << G4endl;
        // G4cout << "stem:           " << path.stem()           << G4endl;
        // G4cout << "extension:      " << path.extension()      << G4endl;

        std::string parent_path = path.parent_path();
        std::string stem = path.stem();
        std::string extension = path.extension();

        // G4cout << "parent_path: " << parent_path << G4endl;
        // G4cout << "stem:        " << stem        << G4endl;
        // G4cout << "extension:   " << extension   << G4endl;

        root_output_path = parent_path + "/" + stem + "_" + run_str + extension;

        // G4cout << "root_output_path: " << root_output_path << G4endl;

    }

    // get run number
    AnalysisManager * analysis_manager = AnalysisManager::Instance();
    // analysis_manager->Book(root_output_path_);
    analysis_manager->Book(root_output_path);
    analysis_manager->SetRun(run->GetRunID());

    // reset event variables
    analysis_manager->EventReset();

    // get MC truth manager
    MCTruthManager * mc_truth_manager = MCTruthManager::Instance();

    // reset event in MC truth manager
    mc_truth_manager->EventReset();
}


void RunAction::EndOfRunAction(const G4Run*)
{
    // get analysis manager
    AnalysisManager * analysis_manager = AnalysisManager::Instance();
	
    // get detector construction
    DetectorConstruction * detector_construction = new DetectorConstruction();

    // get detector dimensions
    double const detector_length_x = detector_construction->GetTargetRadius();
    double const detector_length_y = detector_construction->GetTargetRadius();
    double const detector_length_z = detector_construction->GetTargetLength();

    // save detector dimensions as metadata
    analysis_manager->FillMetadata(detector_length_x,
                                     detector_length_y,
                                     detector_length_z);

    // save run to ROOT file
    analysis_manager->Save();
}

