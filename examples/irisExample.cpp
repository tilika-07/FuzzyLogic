#include "../include/FuzzyLogic.h"
#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <algorithm>
int main() {
	try {
		Dataset iris = CSVReader::read("../../../examples/iris.csv", true, true);
		//printing dataset summary
		std::cout << "Number of samples:"
			<< iris.getNumSamples()
			<< "\n";
		std::cout << "Number of Features:"
			<< iris.getNumFeatures()
			<< "\n";
		SVector fNames= iris.getFeatureNames();
		std::cout << "Feature Names:";
		for (auto& name: fNames) {
			std::cout <<name<<"\n";
		}
		std::cout << "\nFirst Sample: ";
		Vector sample = iris.getSample(0);
		for (double value : sample)
		{
			std::cout << std::fixed
				<< std::setprecision(2)
				<< value
				<< " ";
		}
		std::cout << "\n\nFirst Label\n";
		std::cout << iris.getLabels()[0] << '\n';
		std::cout << "\nDataset successfully loaded.\n";
		
		
		//running fcm
		FuzzyCMeans fcm(3);		//num clusters=3, rest args=default vals
		fcm.fit(iris.getData());
		const Matrix& centers = fcm.getCenters();
		std::cout << "Cluster Centers:\M ";
		const auto& featureNames = iris.getFeatureNames();
		for (size_t i = 0; i < centers.size(); ++i) {
			std::cout << "Cluster" << i + 1<<"\n";
			for (size_t j = 0; j < centers[i].size();++j) {
				std::cout
					<< std::left		//left aligned
					<< std::setw(15)		//fix col width
					<< featureNames[j]<< ": "
					<< std::fixed
					<< std::setprecision(3)
					<< centers[i][j]		//ith cluster,jth feature
					<< '\n';
			}
			std::cout << "\n";
		}
		//printing iterations and convergance 
		std::cout << "\nConverged in "
			<< fcm.getTrainingInfo().iterations
			<< " iterations"<<"\n";
		//printing memberships
		const Matrix& memberships = fcm.getMemberships();
		std::vector<size_t> samples = { 0, 75, 149 };		//random samples from iris.csv
		for (size_t sampleIndex : samples) {
			std::cout << "Sample Number " << sampleIndex + 1 << "(" << iris.getLabels()[sampleIndex] << ")";
			std::cout << "Features: ";
			const auto& sample = iris.getSample(sampleIndex);
			//printing row
			for (double value : sample)
			{
				std::cout << std::fixed
					<< std::setprecision(2)
					<< value
					<< " ";
			}
			std::cout << "\n";
			//printing cluster memebershipn in each cluster of each sample
			for (size_t cluster = 0;cluster < memberships[sampleIndex].size();++cluster)
			{
				std::cout<< " Cluster"<< cluster + 1<< ": "
					<< std::fixed
					<< std::setprecision(4)
					<< memberships[sampleIndex][cluster]
					<< '\n';
			}

			std::cout << '\n';
		}

		//computing cluster purity
		
		double purity =
			ClusteringMetrics::purity(
				fcm.getMemberships(),
				iris.getLabels()
			);

		std::cout
			<< "Cluster Purity: "
			<< std::fixed
			<< std::setprecision(2)
			<< purity * 100.0
			<< "%\n";

	 //converting clusters to fuzzy variables
		std::cout << "Automatic Fuzzy Set Generation\n";
		std::vector<FuzzyVariable> variables;
		for (size_t feature = 0;feature < iris.getNumFeatures();++feature)
		{
			variables.push_back(
				FuzzySetGenerator::generateGaussianVariable(
					iris,
					feature,
					3      // One fuzzy set per cluster for each feature
				)
			);
		}
		for (const auto& variable : variables)
		{
			std::cout << variable.getName() << '\n';
			std::cout << std::string(variable.getName().size(), '-') << '\n';
			for (const auto& set : variable.getSets())
			{
				auto gaussian = dynamic_cast<const Gaussian*>(set.getMembershipFunction());
				if (gaussian == nullptr)
				{
					continue;
				}
				//printing gaussian parameters of set
				std::cout
					<< std::left
					<< std::setw(20)
					<< set.getName()
					<< "Mean = "
					<< std::fixed
					<< std::setprecision(3)
					<< gaussian->getMean()
					<< ", Sigma = "
					<< gaussian->getSigma()
					<< '\n';
			}

			std::cout << '\n';
		}
		//predicting for new flower
		std::cout << "Prediciting for New Sample:\n";
		
		Vector newSample =
		{
			5.5,   // Sepal Length
			3.0,   // Sepal Width
			2.0,   // Petal Length
			0.5    // Petal Width
		};

		std::cout << "Input Sample\n";
		const auto& newFeatureNames = iris.getFeatureNames();
		for (size_t i = 0; i < newSample.size(); ++i)
		{
			std::cout
				<< std::left
				<< std::setw(15)
				<< newFeatureNames[i]
				<< ": "
				<< newSample[i]
				<< '\n';
		}

		Vector sampleMemberships = fcm.predict(newSample);
		std::cout << "\nPredicted Memberships\n";
		size_t bestCluster = 0;
		double highestMembership = sampleMemberships[0];

		for (size_t i = 0; i < sampleMemberships.size(); ++i)
		{
			std::cout
				<< "Cluster "
				<< i + 1
				<< ": "
				<< std::fixed
				<< std::setprecision(4)
				<< sampleMemberships[i]
				<< '\n';

			if (sampleMemberships[i] > highestMembership)
			{
				highestMembership = sampleMemberships[i];
				bestCluster = i;
			}
		}

		std::cout << "\nMost likely cluster: "
			<< bestCluster + 1
			<< '\n';
		std::cout << "Membership in Best Cluster"
			<< sampleMemberships[bestCluster];
	}
	catch(const std::exception& e){
		std::cerr << "Error:"
			<< e.what()
			<< "\n";
		return 1;

		
	}
	
}