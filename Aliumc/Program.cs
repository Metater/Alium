using Alium;

string source = File.ReadAllText(@"C:\Users\Connor\Desktop\Alium Code\basic.ali");
AliumParser aliumParser = new AliumParser(source);
try
{
    Block tree = aliumParser.GetBlockTree();
}
catch (Exception e)
{
    Console.WriteLine(e.Message);
}