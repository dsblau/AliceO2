// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

/// @file   ClusterWriterSpec.cxx

#include <vector>

#include "PHOSWorkflow/ClusterWriterSpec.h"
#include "DPLUtils/MakeRootTreeWriterSpec.h"
#include "DataFormatsPHOS/Cluster.h"
#include "DataFormatsPHOS/MCLabel.h"
#include "DataFormatsPHOS/TriggerRecord.h"
#include "SimulationDataFormat/MCCompLabel.h"
#include "SimulationDataFormat/MCTruthContainer.h"

using namespace o2::framework;

namespace o2
{
namespace phos
{

template <typename T>
using BranchDefinition = MakeRootTreeWriterSpec::BranchDefinition<T>;
using ClusType = std::vector<o2::phos::Cluster>;
using TriggerRecordType = std::vector<o2::phos::TriggerRecord>;
using MCLabelType = o2::dataformats::MCTruthContainer<o2::phos::MCLabel>;
using namespace o2::header;

DataProcessorSpec getClusterWriterSpec(bool useMC)
{
  // Spectators for logging
  // this is only to restore the original behavior
  auto ClustersSize = std::make_shared<int>(0);
  auto ClustersSizeGetter = [ClustersSize](ClusType const& Clusters) {
    *ClustersSize = Clusters.size();
  };

  //  auto logger = [ClustersSize](std::vector<o2::itsmft::ROFRecord> const& rofs) {
//    LOG(INFO) << "ITSClusterWriter pulled " << *compClustersSize << " clusters, in " << rofs.size() << " RO frames";
//  };
  return MakeRootTreeWriterSpec("phos-cluster-writer",
                                "o2clus_phos.root",
                                MakeRootTreeWriterSpec::TreeAttributes{"o2sim", "Tree with PHOS clusters"},
                                BranchDefinition<ClusType>{InputSpec{"clus", "PHS", "CLUSTERS", 0},
                                                               "PHOSCluster",
                                                               ClustersSizeGetter},
                                BranchDefinition<TriggerRecordType>{InputSpec{"clusRecs", "PHS", "CLUSTERTRIGRECS", 0},
                                "PHOSClusterTrigRec"},
                                   
                                BranchDefinition<MCLabelType>{InputSpec{"clusMC", "PHS", "CLUSTERTRUEMC", 0},
                                "PHOSClusterTrueMC",
                                (useMC ? 1 : 0), // one branch if mc labels enabled
                                ""}
                                
)();
}

} // namespace phos
} // namespace o2
