import { Separator } from '@/components/ui/separator'
import { Linkedin, Github } from 'lucide-react'
import { Button } from '@/components/ui/button'

export default function Landing() {
	return (
		<div className="relative left-1/2 top-1/2 -translate-x-1/2 -translate-y-1/2 transform">
			<div className="text-left">
				<h1 className="text-5xl font-medium leading-none 2xl:text-8xl">Faris Rafi Pramana</h1>
				<p className="text-xl text-muted-foreground 2xl:text-2xl">IoT Engineer / Embedded Engineer</p>
				<Separator className="my-6" />
				<div className="flex h-5 items-center space-x-4 text-sm">
					<Button onClick={() => window.open('https://www.linkedin.com/in/farisrafp/', '_blank')}>
						<Linkedin className="mr-2 h-6 w-6" /> LinkedIn
					</Button>
					<Button onClick={() => window.open('https://github.com/farisrfp', '_blank')}>
						<Github className="mr-2 h-6 w-6" /> Github
					</Button>
				</div>
			</div>
		</div>
	)
}
