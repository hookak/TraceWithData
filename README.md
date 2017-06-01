
Usage

1. generator 
	./generator [File] [Entropy] [duplication rate] [Page distribution] [Page Difference]
	ne Paragraph of project description goes here

## Getting Started

	These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites

	What things you need to install the software and how to install them

	```
	Give examples
	```

### Installing

	A step by step series of examples that tell you have to get a development env running

	Say what the step will be

	```
	Give the example
	```

	And repeat

	```
	until finished
	```

	End with an example of getting some data out of the system or using it for a little demo

## Running the tests

	Explain how to run the automated tests for this system

### Break down into end to end tests

	Explain what these tests test and why

	```
	Give an example
	```

### And coding style tests

	Explain what these tests test and why

	```
	Give an example
	```

## Deployment

	Add additional notes about how to deploy this on a live system

## Built With

	* [Dropwizard](http://www.dropwizard.io/1.0.2/docs/) - The web framework used
	* [Maven](https://maven.apache.org/) - Dependency Management
	* [ROME](https://rometools.github.io/rome/) - Used to generate RSS Feeds

## Contributing

	Please read [CONTRIBUTING.md](https://gist.github.com/PurpleBooth/b24679402957c63ec426) for details on our code of conduct, and the process for submitting pull requests to us.

## Versioning

	We use [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/your/project/tags). 

## Authors

	* **Billie Thompson** - *Initial work* - [PurpleBooth](https://github.com/PurpleBooth)

	See also the list of [contributors](https://github.com/your/project/contributors) who participated in this project.

## License

	This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

	* Hat tip to anyone who's code was used
	* Inspiration
	* etc


	[File] <br />
		File name that you make
		
	[Entropy] <br />
		Entropy value is between [0,1]<br />

	[duplication rate] <br />
		rate is between [0,1] <br />

	[Page distribution] <br />
		support two distribution <br />
		0 : Uniform <br />
		1 : Normal <br />

	[Page Difference] <br />
		Difference is between [0,1] <br />
		

2. analyzor
	./analyzor [File]
	
	[File]
		File name that generator made
		

File Format

Uint64_t Request type   <br />
	0 : Read
	1 : Write
	
Uint64_t lpa       <br />
	Unit : 512B(Sector) 
	
Uint64_t size   <br />
	Unit : 512B(Sector)
	
unsigned char data[PAGE_SIZE] <br />
	PAGE_SIZE : 4K
	

 
