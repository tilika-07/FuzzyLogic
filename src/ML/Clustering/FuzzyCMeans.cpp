#include "..\..\..\include\ML\Clustering\FuzzyCMeans.h"
#include "..\..\..\include\ML\Clustering\FuzzyCMeans.h"
#include"../../../include/ML/Clustering/FuzzyCMeans.h"
#include <stdexcept>
#include <cmath>
#include <random>
FuzzyCMeans::FuzzyCMeans(int clusters, double fuzziness, int maxIts, double tolerance,unsigned int seed) :
	numClusters(clusters),
	fuzziness(fuzziness),
	maxIts(maxIts),
	tolerance(tolerance),
	randomSeed(seed) {

	if (clusters <= 0) {
		throw std::invalid_argument("invalud number of clusters");
	}
	if (fuzziness <= 1.0) {
		throw std::invalid_argument("fuzziness must be greater than 1.0");
	}
	if (maxIts <= 0) {
		throw std::invalid_argument("maximum iterations must be positive");
	}
	if (tolerance <= 0) {
		throw std::invalid_argument("tolerance iterations must be positive");
	}
}

Matrix FuzzyCMeans::getCenters() const{
	return centers;
}
Matrix FuzzyCMeans::getMemberships()const{
	return memberships;
}
double FuzzyCMeans::calculateDist(const Vector& a,const Vector& b)const {
	if (a.size() != b.size()) {
		throw std::invalid_argument("vectors must be same size");
	}
	double sum = 0.0;
	for (size_t i = 0; i <a.size(); i++) {
		double diff = a[i] - b[i];
		sum += diff * diff;

	}
	return std::sqrt(sum);
}
void FuzzyCMeans::initialiseMemberships(int numDatapts) {
	memberships.clear();
	memberships.resize(numDatapts, Vector(numClusters));
	std::mt19937 generator(randomSeed);
	std::uniform_real_distribution<double> distribution(0.0, 1.0);
	for (int i = 0; i <numDatapts; i++) {
		double sum = 0.0;
		for (int j = 0; j < numClusters; j++) {
			memberships[i][j] = distribution(generator);
			sum += memberships[i][j];
		}
		for (int j = 0; j < numClusters; j++) {
			memberships[i][j] /= sum;
		}
	}
}
	
void FuzzyCMeans::updateCenters(const Matrix& data) {
	int numFeatures = data[0].size();
	centers.clear();
	centers.resize(numClusters, std::vector<double>(numFeatures));
	for (int cluster = 0; cluster < numClusters; cluster++) {
		double denominator = 0.0;
		for (int point = 0; point < data.size(); point++) {
			double weight = std::pow(memberships[point][cluster], fuzziness);
			denominator += weight;
			for (int feature = 0; feature < numFeatures; feature++) {
				centers[cluster][feature] += weight * data[point][feature];
			}
		}
		for (int feature = 0; feature < numFeatures; feature++)
		{
			centers[cluster][feature] /= denominator;
		} }
}

void FuzzyCMeans::updateMemberships(
	const Matrix& data
)
{
	for (int i = 0; i < data.size(); i++)
	{
		int exactCluster = -1;


		// check zero distance case
		for (int j = 0; j < numClusters; j++)
		{
			double dist =
				calculateDist(
					data[i],
					centers[j]
				);


			if (dist == 0.0)
			{
				exactCluster = j;

				break;
			}
		}


		if (exactCluster != -1)
		{
			for (int j = 0; j < numClusters; j++)
			{
				memberships[i][j] = 0.0;
			}


			memberships[i][exactCluster] = 1.0;

			continue;
		}


		// normal FCM update
		for (int j = 0; j < numClusters; j++)
		{
			double denominator = 0.0;


			double distToCurr =
				calculateDist(
					data[i],
					centers[j]
				);


			for (int k = 0; k < numClusters; k++)
			{
				double distToOther =
					calculateDist(
						data[i],
						centers[k]
					);


				denominator += std::pow(
					distToCurr / distToOther,
					2.0 / (fuzziness - 1)
				);
			}


			memberships[i][j] =
				1.0 / denominator;
		}
	}
}
bool FuzzyCMeans::hasConverged(Matrix& oldMemberships) {
	return calcMaxDifference(oldMemberships) < tolerance;
}
void FuzzyCMeans::fit(const Matrix& data) {
	if (data.empty()) {
		throw std::invalid_argument("Data can not be empty.");
	}
	if (data[0].empty())
	{
		throw std::invalid_argument(
			"Data points cannot be empty"
		);
	}
	if (numClusters > data.size())
	{
		throw std::invalid_argument(
			"Clusters cannot exceed data points"
		);
	}
	int numFeatures = data[0].size();
	for (auto& point : data) {
		if (point.size() != numFeatures)
		{
			throw std::invalid_argument("All data points must have same number of features");
		} }
	initialiseMemberships(data.size());
	for (int iteration = 0; iteration < maxIts; iteration++) {
		auto oldMemberships = memberships;
		updateCenters(data);
		updateMemberships(data);
		if (hasConverged(oldMemberships)) {
			traininginfo.iterations = iteration + 1;
			traininginfo.hasConverged = true;
			traininginfo.finalError = calcMaxDifference(oldMemberships);
			break;
		}
	}
	trained = true;
}
Vector FuzzyCMeans::predict(const Vector& point) const {
	if (!trained) {
		throw std::runtime_error("Model must be fitted first.");
	}
	if (point.size() != centers[0].size()) {
		throw std::invalid_argument("Point dimension does not match training data.");
	}
	std::vector<double> res(numClusters, 0.0);
	for (int j = 0; j < numClusters; j++)
	{
		double distanceCurrent =
			calculateDist(
				point,
				centers[j]
			);
		// exact match with cluster center
		if (distanceCurrent == 0.0)
		{
			res[j] = 1.0;
			return res;
		}
		double denominator = 0.0;
		for (int k = 0; k < numClusters; k++)
		{
			double distanceOther =
				calculateDist(
					point,
					centers[k]
				);
			denominator += std::pow(
				distanceCurrent / distanceOther,
				2.0 / (fuzziness - 1)
			);
		}
		res[j] =
			1.0 / denominator;
	}
	return res;
}
int FuzzyCMeans::predictCluster(const Vector& point) const {
	Vector res= predict(point);
	return static_cast<int>(std::distance(memberships.begin(), std::max_element(memberships.begin(), memberships.end())));

}
FuzzyCMeans::TrainingInfo FuzzyCMeans::getTrainingInfo() const {
	return traininginfo;
}
double FuzzyCMeans::calcMaxDifference(const Matrix& oldMemberships)const
{
	double maxDiff = 0.0;
	for (int i = 0; i < memberships.size(); i++)
	{
		for (int j = 0; j < numClusters; j++)
		{
			double diff =
				std::abs(
					memberships[i][j] -
					oldMemberships[i][j]
				);

			maxDiff = std::max(maxDiff, diff);
		}
	}
return maxDiff;
}