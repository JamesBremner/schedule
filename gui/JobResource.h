class cJobType
{
public:
    cJobType()
    {

    }
    cJobType( const std::string& name, int crew )
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

class cJob
{
public:
    cJob( const std::string& type,
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

class cResourceType
{
public:
    cResourceType()
    {

    }
    cResourceType( const std::string& type )
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

class cResource
{
public:
    cResource( const std::string& name,
               const std::string& type )
        : myName( name )
        , myType( type )
        , myfAssign( false )
        , myLastShift( -10 )
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

    void Assign( bool f, int shift = -1 );

    bool Assign() const
    {
        return myfAssign;
    }
    int LastShift() const
    {
        return myLastShift;
    }

    /// True if available for a shift
    bool IsAvailable( int shift ) const;

private:
    std::string myName;
    std::string myType;
    bool myfAssign;
    int myLastShift;
};

class cFleet
{
public:
    cFleet( nana::form& fm )
        : myfm( fm )
        , fleet_text( fm, nana::rectangle( 10,100, 250, 350 ))
    {

    }
    void Read();
    void Write();
    void Add( const cJob& v )
    {
        myJobVector.push_back( v );
    }

    void NewJobType( const nana::form& fm )
    {
        std::vector<std::string> type_names;
        for( auto& t : myResourceTypeVector )
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
            cJobType vt;
            if( FindType( name.value(), vt ))
            {
                nana::msgbox msg("Already have this vehicle type");
                msg.show();
            }
            else
            {
                myTypeVector.push_back( cJobType(
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
            cResourceType pt;
            if( FindPersonType( name.value(), pt ))
            {
                nana::msgbox msg("Already have this person type");
                msg.show();
            }
            else
            {
                myResourceTypeVector.push_back( cResourceType(
                                                  name.value() ) );
            }
        }
    }
    void NewJob();

    void NewResource( nana::form& fm )
    {
        std::vector<std::string> type_names;
        for( auto& t : myResourceTypeVector )
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
                myResourceVector.push_back( cResource( name.value(), type.value() ));
            }
        }

    }
    void Display(  )
    {
        fleet_text.select( true );
        fleet_text.del();
        fleet_text.append(std::to_string( myJobVector.size())+ " " + myJobTerm + ":\n", false);
        for( auto& v : myJobVector )
        {
            cJobType vt;
            FindType( v.Type(), vt );
            std::vector<std::string> vn = vt.CrewType();
            fleet_text.append( v.Plate()+" "+v.Type()+" needs ", false );
            for( int kct=0; kct<vt.Crew(); kct++)
                fleet_text.append( vn[kct]+" ",false);
            fleet_text.append("\n",false);
        }
        fleet_text.append("\n" + std::to_string( myResourceVector.size())+ " " + myResourceTerm + ":\n", false);
        for( auto& p : myResourceVector )
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
        for( int kshift = 0; kshift < shifts; kshift++ )
        {
            // clear previous shift's assignments
            for( auto& p : myResourceVector )
            {
                p.Assign( false );
            }

            // the assignments for this shift
            std::vector< std::pair< cJob, cResource > > shift_assignments;

            // loop over vehicles
            for( auto& v : myJobVector )
            {
                cJobType vt;
                if( ! FindType( v.Type(), vt ) )
                    throw std::runtime_error("Vehicle type error");

                // loop over people needed by vehicle type
                for( int kp = 0; kp < vt.Crew(); kp++ )
                {
                    bool success = false;

                    // loop over people
                    for( auto& p : myResourceVector )
                    {
                        if( ! p.IsAvailable( kshift ) )
                            continue;
                        if( p.Type() != vt.CrewType()[kp] )
                            continue;

                        // assign person to vehicle
                        shift_assignments.push_back( std::pair< cJob, cResource >( v, p ) );
                        p.Assign( true, kshift );
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

            std::random_shuffle( myResourceVector.begin(), myResourceVector.end() );
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
        myResourceTypeVector.push_back( cResourceType("officer"));
        myResourceTypeVector.push_back( cResourceType("firefighter"));
        myTypeVector.push_back( cJobType("rescue",2));
        myTypeVector.back().CrewType({"officer","firefighter"});
        myJobVector.push_back( cJob("rescue","A"));
        myResourceVector.push_back( cResource("Alice","officer"));
        myResourceVector.push_back( cResource("Bob","firefighter"));
    }

private:

    std::string myJobTerm;
    std::string myResourceTerm;
    std::vector< cJob > myJobVector;
    std::vector< cJobType > myTypeVector;
    std::vector< cResource > myResourceVector;
    std::vector< cResourceType > myResourceTypeVector;
    std::vector< std::vector< std::pair< cJob, cResource > > > myAssign;
    nana::form& myfm;
    nana::textbox fleet_text;

    bool FindType( const std::string& type_name,
                   cJobType& type )
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
                         cResourceType& type )
    {
        for( auto& t : myResourceTypeVector )
            if( t.Type() == type_name )
            {
                type = t;
                return true;
            }
        return false;
    }
    bool FindPerson( const std::string& type )
    {
        for( auto& p : myResourceVector )
            if( p.Name() == type )
                return true;
        return false;
    }
};

