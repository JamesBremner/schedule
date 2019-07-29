class cVehicleType
{
public:
    cVehicleType()
    {

    }
    cVehicleType( const std::string& name, int crew )
        : myName( name)
        , myCrew( crew )
    {

    }
    std::string Name()
    {
        return myName;
    }
    int Crew()
    {
        return myCrew;
    }
    void CrewType( const std::vector< std::string >& type )
    {
        myCrewType = type;
    }
    std::vector< std::string >& CrewType()
    {
        return myCrewType;
    }
private:
    std::string myName;
    int myCrew;
    std::vector< std::string > myCrewType;
};

class cVehicle
{
public:
    cVehicle( const std::string& type,
              const std::string& plate )
        : myType( type )
        , myPlate( plate )
    {

    }
    std::string Type()
    {
        return myType;
    }
    std::string Plate()
    {
        return myPlate;
    }

private:
    std::string myType;
    std::string myPlate;
};

class cPersonType
{
public:
    cPersonType()
    {

    }
    cPersonType( const std::string& type )
        : myType( type )
    {

    }
    std::string Type()
    {
        return myType;
    }
private:
    std::string myType;
};

class cPerson
{
public:
    cPerson( const std::string& name,
             const std::string& type )
        : myName( name )
        , myType( type )
        , myfAssign( false )
    {

    }
    std::string Name()
    {
        return myName;
    }
    std::string Type()
    {
        return myType;
    }
    std::string Text()
    {
        return myType + " " + myName;
    }
    void Assign( bool f )
    {
        myfAssign = f;
    }
    bool Assign()
    {
        return myfAssign;
    }
private:
    std::string myName;
    std::string myType;
    bool myfAssign;
};

class cFleet
{
public:
    cFleet( nana::form& fm )
    : fleet_text( fm, nana::rectangle( 10,100, 250, 350 ))
    {

    }
    void Add( const cVehicle& v )
    {
        myFleet.push_back( v );
    }

    void NewJobType( const nana::form& fm )
    {
        std::vector<std::string> type_names;
        for( auto& t : myPersonTypeVector )
        {
            type_names.push_back( t.Type() );
        }
        nana::inputbox::text name("Name", "");
        nana::inputbox::integer crew("Number of crew",3,1,10,1);
        nana::inputbox::text crewType1("Crew Type 1", type_names );
        nana::inputbox::text crewType2("Crew Type 2", type_names );
        nana::inputbox::text crewType3("Crew Type 3", type_names );
        nana::inputbox::text crewType4("Crew Type 4", type_names );
        nana::inputbox::text crewType5("Crew Type 5", type_names );
        nana::inputbox inbox( fm, "New " + JobTerm() + " Type" );

        if( inbox.show( name, crew
                        , crewType1
                        , crewType2
                        , crewType3
                        , crewType4
                        , crewType5 ) )
        {
            cVehicleType vt;
            if( FindType( name.value(), vt ))
            {
                nana::msgbox msg("Already have this vehicle type");
                msg.show();
            }
            else
            {
                myTypeVector.push_back( cVehicleType(
                                            name.value(),
                                            crew.value() ) );
                std::vector< std::string > types;
                types.push_back( crewType1.value() );
                types.push_back( crewType2.value() );
                types.push_back( crewType3.value() );
                types.push_back( crewType4.value() );
                types.push_back( crewType5.value() );
                myTypeVector.back().CrewType( types );
            }
        }
    }
    void NewResourceType( nana::form& fm )
    {
        nana::inputbox::text name("Name of type", "");

        nana::inputbox inbox( fm, "New Person Type" );

        if( inbox.show( name ) )
        {
            cPersonType pt;
            if( FindPersonType( name.value(), pt ))
            {
                nana::msgbox msg("Already have this person type");
                msg.show();
            }
            else
            {
                myPersonTypeVector.push_back( cPersonType(
                                                  name.value() ) );
            }
        }
    }
    void NewJob( nana::form& fm )
    {
        std::vector<std::string> type_names;
        for( auto& t : myTypeVector )
        {
            type_names.push_back( t.Name() );
        }
        nana::inputbox::text type("Type", type_names );
        char plate_default[] { 65 + myFleet.size(), 0 };
        nana::inputbox::text plate("Plate", std::string(plate_default));
        nana::inputbox inbox( fm, myJobTerm + " Type to add" );
        if( inbox.show(type, plate ) )
        {
            myFleet.push_back( cVehicle( type.value(), plate.value() ));
        }

    }
    void NewResource( nana::form& fm )
    {
        std::vector<std::string> type_names;
        for( auto& t : myPersonTypeVector )
        {
            type_names.push_back( t.Type() );
        }
        nana::inputbox::text name("Name", "");
        nana::inputbox::text type("Type", type_names );
        nana::inputbox inbox( fm, "New " + myResourceTerm );
        if( inbox.show( name, type ) )
        {
            if( FindPerson( name.value() ))
            {
                nana::msgbox msg("Already have person with this name");
                msg.show();
            }
            else
            {
                myPersonVector.push_back( cPerson( name.value(), type.value() ));
            }
        }

    }
    void Display(  )
    {
        fleet_text.select( true );
        fleet_text.del();
        fleet_text.append(std::to_string( myFleet.size())+ " " + myJobTerm + ":\n", false);
        for( auto& v : myFleet )
        {
            cVehicleType vt;
            FindType( v.Type(), vt );
            std::vector<std::string> vn = vt.CrewType();
            fleet_text.append( v.Plate()+" "+v.Type()+" needs ", false );
            for( int kct=0; kct<vt.Crew(); kct++)
                fleet_text.append( vn[kct]+" ",false);
            fleet_text.append("\n",false);
        }
        fleet_text.append("\n" + std::to_string( myPersonVector.size())+ " " + myResourceTerm + ":\n", false);
        for( auto& p : myPersonVector )
        {
            fleet_text.append( p.Text() + "\n", false );
        }
        if( myAssign.size() )
        {
            int ks = 1;
            for( auto& sa : myAssign )
            {
                fleet_text.append("\nSchedule Shift "+std::to_string(ks++)+":\n",
                                  false);
                for( auto& a : sa )
                {
                    fleet_text.append(
                        a.second.Name()+" in "+a.first.Type()+" plate "+a.first.Plate()+" "+"\n",
                        false);
                }
            }
        }
    }

    bool Schedule( int shifts )
    {
        // clear all previous assignments
        myAssign.clear();

        // loop over shifts
        for( int ks = 0; ks < shifts; ks++ )
        {
            // clear previous shift's assignments
            for( auto& p : myPersonVector )
            {
                p.Assign( false );
            }

            // the assignments for this shift
            std::vector< std::pair< cVehicle, cPerson > > shift_assignments;

            // loop over vehicles
            for( auto& v : myFleet )
            {
                cVehicleType vt;
                if( ! FindType( v.Type(), vt ) )
                    throw std::runtime_error("Vehicle type error");

                // loop over people needed by vehicle type
                for( int kp = 0; kp < vt.Crew(); kp++ )
                {
                    bool success = false;

                    // loop over people
                    for( auto& p : myPersonVector )
                    {
                        if( p.Assign() )
                            continue;
                        if( p.Type() != vt.CrewType()[kp] )
                            continue;

                        // assign person to vehicle
                        shift_assignments.push_back( std::pair< cVehicle, cPerson >( v, p ) );
                        p.Assign( true );
                        success = true;
                        break;
                    }
                    if( ! success )
                    {
                        nana::msgbox msg("Not enough people to crew the fleet");
                        msg.show();
                        myAssign.clear();
                        return false;
                    }
                }
            }
            myAssign.push_back( shift_assignments );

            std::random_shuffle( myPersonVector.begin(), myPersonVector.end() );
        }
        return true;
    }

    void JobTerm( const std::string& term )
    {
        myJobTerm = term;
    }
    std::string JobTerm() const
    {
        return myJobTerm;
    }
    void ResourceTerm( const std::string& term )
    {
        myResourceTerm = term;
    }
    std::string ResourceTerm() const
    {
        return myResourceTerm;
    }
    void Test()
    {
        myPersonTypeVector.push_back( cPersonType("officer"));
        myPersonTypeVector.push_back( cPersonType("firefighter"));
        myTypeVector.push_back( cVehicleType("rescue",2));
        myTypeVector.back().CrewType({"officer","firefighter"});
        myFleet.push_back( cVehicle("rescue","A"));
        myPersonVector.push_back( cPerson("Alice","officer"));
        myPersonVector.push_back( cPerson("Bob","firefighter"));
    }

private:

    std::string myJobTerm;
    std::string myResourceTerm;
    std::vector< cVehicle > myFleet;
    std::vector< cVehicleType > myTypeVector;
    std::vector< cPerson > myPersonVector;
    std::vector< cPersonType > myPersonTypeVector;
    std::vector< std::vector< std::pair< cVehicle, cPerson > > > myAssign;
    nana::textbox fleet_text;

    bool FindType( const std::string& type_name,
                   cVehicleType& type )
    {
        for( auto& t : myTypeVector )
            if( t.Name() == type_name )
            {
                type = t;
                return true;
            }
        return false;
    }
    bool FindPersonType( const std::string& type_name,
                         cPersonType& type )
    {
        for( auto& t : myPersonTypeVector )
            if( t.Type() == type_name )
            {
                type = t;
                return true;
            }
        return false;
    }
    bool FindPerson( const std::string& type )
    {
        for( auto& p : myPersonVector )
            if( p.Name() == type )
                return true;
        return false;
    }
};

