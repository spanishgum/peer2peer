#include "../hdr/node.h"
// #include "../hdr/client.h"
// #include "../hdr/server.h"

void fatal(string s) {
	if (errno)
		perror(s.c_str());
	else
		cerr << "Error: " << s << " . . .\n";
	exit(-1);	
}

// -- utilities -------------------------------------------
string trim(const string &str, const string &delims);


/**********************************************************
* ------------------- CONSTRUCTION -----------------------
**********************************************************/

Node::Node() {
	this->neighborPort = 0;
	this->filePort = 0;
	this->numberOfPeers = 0;
	this->TTL = 0;
	this->isSeedNode = false;
	this->seedNodes = "seedNodes.dat";
	this->localFilesDirectory = "files/";
	this->localFiles = "localFiles.dat";
	this->err_msg = "";
}

Node::~Node() {

}


/**********************************************************
* ---------------------- GETTERS -------------------------
**********************************************************/

int Node::get_neighborPort() {
	return this->neighborPort;
}
int Node::get_filePort() {
	return this->filePort;
}
int Node::get_numberOfPeers() {
	return this->numberOfPeers;
}
int Node::get_TTL() {
	return this->TTL;
}
string Node::get_seedNodes() {
	return this->seedNodes;
}
bool Node::get_isSeedNode() {
	return this->isSeedNode;
}
string Node::get_localFilesDirectory() {
	return this->localFilesDirectory;
}
string Node::get_localFiles() {
	return this->localFiles;
}


/**********************************************************
* ---------------------- SETTERS -------------------------
**********************************************************/

bool Node::set_neighborPort(int p) {
	cerr << "test" << endl;
	if (!this) {
		cerr << "test" << endl;
	}
	
	if (p < PORT_MIN || p > PORT_MAX
		|| p == this->filePort)
		return false;
	this->neighborPort = p;
	return true;
}

bool Node::set_filePort(int p) {
	if (p < PORT_MIN || p > PORT_MAX
		|| p == this->filePort)
		return false;
	this->filePort = p;
	return true;
}
bool Node::set_numberOfPeers(int n) {
	if (n < 0 || n > PEER_LIMIT)
		return false;
	this->numberOfPeers = n;
	return true;
}
bool Node::set_TTL(int hops) {
	if (hops < 1)
		return false;
	this->TTL = hops;
	return true;
}
bool Node::set_seedNodes(string fname) {
	struct stat sb;
	if (stat(fname.c_str(), &sb) == 0 && S_ISREG(sb.st_mode)) {
		this->seedNodes = fname;
		return true;
	}
	else return false;
}


bool Node::set_isSeedNode(bool b) {
	this->isSeedNode = b;
	return true;
}

bool Node::set_localFilesDirectory(string dname) {
	struct stat sb;
	if (stat(dname.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode)) {
		this->localFilesDirectory = dname;
		return true;
	}
	else return false;
}

bool Node::set_localFiles(string fname) {
	struct stat sb;
	if (stat(fname.c_str(), &sb) == 0 && S_ISREG(sb.st_mode)) {
		this->localFilesDirectory = fname;
		return true;
	}
	else return false;
}



/**********************************************************
* ---------------------- READERS -------------------------
**********************************************************/

int Node::read_seeds() {
	string fname = this->get_seedNodes();
	ifstream ifs;
	string line, host;
	int port;
	sstream ss;
	ifs.open(fname, ios::in);
	if (!ifs) {
		cerr << "Failed to open " << fname << endl;
		return -1;
	}
	while (getline(ifs, line)) {
		ss << line;
		ss >> host >> port;
		if (ss.fail()) {
			cerr << "Bad input in " << fname << ":\n\t"
				<< line << endl;
		}
		else {
			this->seeds.push_back(make_pair(host, port));
		}
		ss.str(string());
	}
	return 0;
}

int Node::read_catalogue() {
	string fname = this->get_localFiles(),
		line, file, key;
	list<string> keywords;
	ifstream ifs;
	sstream ss;
	ifs.open(fname, ios::in);
	if (!ifs) {
		cerr << "Failed to open " << fname << endl;
		return -1;
	}
	while (getline(ifs, line)) {
		ss << line;
		getline(ss, file, ':');
		while (getline(ss, key, '|')) {
			keywords.push_back(trim(key, " \t"));
		}
		this->catalogue.push_back(
			make_pair(trim(file, " \t"), keywords));
		keywords.clear();
		ss.str(string());
	}
	return 0;
}


/**********************************************************
* ---------------------- RUNNING -------------------------
**********************************************************/

void Node::init(int opt) {
	switch (opt) {
		case 0:
			this->client = new Client(this->neighborPort);
			this->client->init(*this);
			break;
		case 1:
			if (this->isSeedNode) {
				this->server = new Server(this->filePort);	
				this->server->init(*this);
			}
			break;
		default:
			cout << "Invalid init option.\n";
	}
}

#define mask(n) ((1 << (n)) - 1)

char *Node::generate_hdr(int protocol, int payload) {
	char msg_hdr[23];
	// sprintf(msg_hdr, "%.*s%c%c%c%.*s",
		// 16, rand(),
		// protocol,
		// this->TTL,
		// 0,
		// 4, payload);
	
	for (int i = 0; i < 16; ++i)
		msg_hdr[i] = rand();
	msg_hdr[16] = protocol;
	msg_hdr[17] = this->TTL;
	msg_hdr[18] = 0;
	for (int i = 19; i < 23; ++i)
		msg_hdr[i] = ((payload >> (i - 19)) & mask(8));
	
	cout << "1: " << msg_hdr[19] << endl;
	cout << "1: " << ((payload >> 0) & mask(8)) << endl;
	cout << "2: " << msg_hdr[20] << endl;
	for (int i = 19; i < 23; ++i)
		cout << ((payload >> (i - 19)) & mask(8)) << endl;
	cout << "----------\n" << payload << " " << mask(8) << endl;
	cout << "----------\n" << ((payload >> 1) & mask(8)) << endl;
	// sstream ss("");
	// ss.write(rand(), 16);
	// ss.write(protocol, 1);
	// ss.write(this->TTL, 1);
	// ss.write(0);
	// ss.write(payload, 4);
	// return ss.str().c_str();
	return msg_hdr;
}

/**********************************************************
* ---------------------- UTILITY -------------------------
**********************************************************/

string trim(const string &str, const string &delims) {
	auto beg = str.find_first_not_of(delims);
	auto end = str.find_last_not_of(delims);
	return str.substr(beg, (end - beg + 1));
}

void Node::show_conf() {
	cout << "\nneighborPort: " << get_neighborPort()
		<< "\nfilePort: " << get_filePort()
		<< "\nnumberOfPeers: " << get_numberOfPeers()
		<< "\nTTL: " << get_TTL()
		<< "\nseedNodes: " << get_seedNodes()
		<< "\nisSeedNode: " << get_isSeedNode()
		<< "\nlocalFilesDirectory: " << get_localFilesDirectory()
		<< "\nlocalFiles: " << get_localFiles() << "\n\n";
}
